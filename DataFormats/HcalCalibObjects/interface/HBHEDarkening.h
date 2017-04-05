#ifndef HcalCalibObjects_HBHEDarkening_h
#define HcalCalibObjects_HBHEDarkening_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>
#include <map>

// Scintillator darkening model for HB and HE
// ingredients:
// 1) dose map (Mrad/fb-1), from Fluka
// 2) decay constant D as function of dose rate d (Mrad vs krad/hr): D(d) = A*d^B
// 3) inst lumi per year (fb-1/hr)
// 4) int lumi per year (fb-1)
// layer number for HB: (0,16) = (1,17) in HcalTestNumbering
// layer number for HE: (-1,17) = (1,19) in HcalTestNumbering

class HBHEDarkening {
	public:
		HBHEDarkening(const edm::ParameterSet & p);
		~HBHEDarkening() {}
		
		//public accessor
		double degradation(double intlumi, int ieta, int lay) const;
		
		//helper function
		static std::vector<std::vector<double>> readDoseMap(const std::string& fullpath);

		//helper classes
		struct LumiYear {
			//constructors
			LumiYear() : 
				year_(""), intlumi_(0.), lumirate_(0.), energy_(0), sumlumi_(0.) {}
			LumiYear(std::string year, double intlumi, double lumirate, int energy) : 
				year_(year), intlumi_(intlumi), lumirate_(lumirate), energy_(energy), sumlumi_(0.) {}
			LumiYear(const edm::ParameterSet & p) : 
				year_(p.getParameter<std::string>("year")), intlumi_(p.getParameter<double>("intlumi")), lumirate_(p.getParameter<double>("lumirate")), energy_(p.getParameter<int>("energy")), sumlumi_(0.) {}
			
			//sorting
			bool operator<(const LumiYear& yr) const {
				return year_ < yr.year_;
			}
			
			//member variables
			std::string year_;
			double intlumi_;
			double lumirate_;
			int energy_;
			double sumlumi_;
		};
		struct LumiYearComp {
			bool operator()(const LumiYear& yr, const double& lum) const {
				return yr.sumlumi_ < lum;
			}
		};

	private:
		//helper functions
		double dose(int ieta, int lay, int energy) const;
		std::string getYearForLumi(double intlumi) const;
		double degradationYear(const LumiYear& year, double intlumi, int ieta, int lay) const;
	
		//member variables
		int ieta_shift_;
		double drdA_, drdB_;
		std::map<int,std::vector<std::vector<double>>> dosemaps_; //one map for each center of mass energy
		std::vector<LumiYear> years_;
};

#endif // HBHEDarkening_h
