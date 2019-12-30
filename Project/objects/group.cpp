#include "group.h"

Group::Group(std::string type, int damount):dayAmount(damount){
	generator.seed(rand());
	Group::type = type;
	Wno = 20000;
	W1 = 10000;
}


void Group::addMember( Labor * labor){
#ifdef DEBUG
	if (members.count(labor->Name()))
		std::cout<<colored(labor->Name() + " is duplicate",fontstyle::RED);
#endif
	members[labor->Name()] = labor;
	vectorOfmembers.push_back(labor);
}

void Group::setUpHoliday(){
	int i = 0;
	// cout<<"Group is "<<type<<endl;
	for(map<string, Labor *>::iterator it = members.begin(), end = members.end(); it != end; it++, i++)
		it->second->setHoliday(i);
	selector = new uniform_int_distribution<int>(0,members.size() - 1);
}

int Group::PeopleWorkDays(unsigned int people_amount){
//	cout<<"members.size = "<<members.size()<<endl;
	int days = 0;
	unsigned int howManyPeopleWork;
	for(int i = 0; i < dayAmount; ++i){
		howManyPeopleWork = 0;
		for(map<string, Labor*>::iterator it = members.begin(), end = members.end(); it != end; it++){
			if(it->second->isWoringThisDay(i)){
				++howManyPeopleWork;	
			}
		}
		if(howManyPeopleWork == people_amount)
			++days;
//		cout<<"==================="<<endl;
	}
	return days;
}

double Group::ComputationGroupQuality(){
	double quility = 0.0;
	double PQmax_PQmin = 0.0;
	int PQ = 0;
	int PQmax, PQmin;
	PQmax = 1;
	PQmin = 1000000;
	int noPeopleWork = PeopleWorkDays();
	int onePeopleWork = PeopleWorkDays(1);
	quility += Wno * noPeopleWork;
	quility += W1 * onePeopleWork;
	for(std::map<std::string, Labor *>::iterator it = members.begin(), end = members.end(); it != end; it++){
		PQ = it->second->ComputationPersonalQuality();
		PQmax_PQmin += PQ;
		if(PQ > PQmax){
			PQmax = PQ;
		}
		if(PQ < PQmin){
			PQmin = PQ;
		}
	}
	// cout<<"PQ max = "<<PQmax<<" PQmin = "<<PQmin<<endl;
	PQmax_PQmin /= 3.0;
	quility += PQmax;
	// printf("Hno = %d, H1 = %d,(PQmax - PQmin) / 3 = %f,  Quality = %f\n",noPeopleWork, onePeopleWork,PQmax_PQmin, quility);	
	return quility;
}

void Group::randomlySelectLaborSwapTheDay(){
	int rnd = selector->operator()(generator);
	Labor * labor = vectorOfmembers[rnd];
	labor->randomlySwapDayType();
	lastSelectedLabor = labor;
}

void Group::laborScheduleRestore(){
	if(lastSelectedLabor)
		lastSelectedLabor->restoreLastSchedule();
}
