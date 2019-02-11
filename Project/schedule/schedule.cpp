
#ifndef __SCHEDULE_CPP__
#define __SCHEDULE_CPP__

#include "schedule.hpp"


Schedule::Schedule(int mon){
    month = mon;
    deque<String> calendar;
    deque<String> weight;
    deque<String> last;

    calendar = load(("../Files/calendar" + to_string(month) + ".csv"));
    weight = load("../Files/day.weight" + to_string(month) + ".csv");
    last = load("../Files/calendar" + to_string(month - 1) + ".csv");


    deque<deque<String> > calendarContent = linesSplit(calendar);
    deque<deque<String> > weightContent = linesSplit(weight);
    deque<deque<String> > lastContent = linesSplit(last);

    lastCalendarProcess(lastContent);

    dayAmounts = calendarContent[0].size() - 1 + lastContent[0].size() - 1;

    createScheduleDataStructure(calendarContent, weightContent, lastContent);

//
//    for(int i = 0; i < calendarContent.size(); i++){
//        for(int j = 0; j < calendarContent[i].size(); j++){
//            cout<<calendarContent[i][j];
//        }
//        cout<<endl;
//    }

}

deque<String> Schedule::load(string file_name){
    ifstream file;
    String sline;
    deque<String> lines;
    
    file.open(file_name , std::fstream::in);
    if(file.is_open()){
        while(!file.eof()){
            file>>sline;
            lines.push_back(sline);
        }
    }else{
        // Here throw exception, use try-catch to handle and generate the new calender for the correct month
        string f = ("calender" + to_String(month) + ".csv");
        fstream::failure e(f + " open failure, Please check the file name");
        throw e;
    }
    return lines;
}


deque<deque<String> > Schedule::linesSplit(deque<String> lines){
    deque<deque<String> > content;
    for(int i = 0; i < lines.size(); i++){
        content.push_back(lines[i].split(","));
    }
    return  content;
}

void Schedule::createScheduleDataStructure(deque<deque<String> > calendarContent, deque<deque<String> > weightContent,
                                           deque<deque<String> > lastContent) {

    deque<String> dt = calendarContent.pop_front();
    dt.pop_front();// pop_front for the first dummy string
    date = dt;

    deque<String> dy = calendarContent.pop_front();
    dy.pop_front();// pop front for the first dummy string
    day = dy;



    weightContent.pop_front(2);


    deque<String> ldt = lastContent.pop_front();
    deque<String> ldy = lastContent.pop_front();


    // pop front for the first dummy string;
    ldt.pop_front();
    ldy.pop_front();


    Map<String,deque<String> > calendarCat = calendarConcat(lastContent, calendarContent);

    createBase(calendarCat,dt,dy,ldt,ldy);
}

void Schedule::lastCalendarProcess(deque<deque<String> > &lastContent) {
//    lastContent.pop_front(2); // pop the day and date;

    for(int i = 0; i < lastContent.size(); i++){
        String name = lastContent[i].pop_front();
//        cout<<"name = "<<name<<endl;
        lastContent[i].pop_front(lastContent[i].size() - 7);
        lastContent[i].push_front(name);
//        cout<<lastContent[i].size()<<endl;
    }

//    for(int i = 0; i < lastContent.size(); i++){
//        for(int j = 0 ; j < lastContent[i].size(); j++){
//            cout<<lastContent[i][j];
//        }
//        cout<<endl;
//    }

}


Map<String, deque<String> > Schedule::calendarConcat(deque<deque<String> > last, deque<deque<String> > now) {
    Map<String, deque<String> >  qMaptolast;
    Map<String, deque<String> >  qMaptocurrent;
//    Map<String, deque<String> > concat;

    for(int i = 0; i < now.size(); i++){
        String name = now[i].pop_front();
        qMaptocurrent[name] = now[i];
    }

    for(int i = 0; i < last.size(); i++){
        String name = last[i].pop_front();
        qMaptolast[name] = last[i];
    }

    for(Map<String, deque<String> >::iterator it = qMaptolast.begin(); it != qMaptolast.end(); it++){
        qMaptolast[it->first] += qMaptocurrent[it->first];
    }

    return qMaptolast;
}


void Schedule::createBase(Map<String, deque<String> > calendarcat, deque<String> cdate, deque<String> cday, deque<String> ldate,
                          deque<String> lday) {

    deque<cDay> dDtemp;
    // last month:
    for(Map<String, deque<String> > ::iterator msdsit = calendarcat.begin(); msdsit != calendarcat.end(); msdsit++){
        dDtemp.clear();
//        cout<<"Name is "<<msdsit->first<<endl;
        for(int i = 0; i < ldate.size(); i++){
            cDay dtemp(lday[i],ldate[i],month - 1, msdsit->second[i],0);
            dtemp.setExpectAttr();
            dDtemp.push_back(dtemp);
//            cout<<dtemp<<endl;
        }
        base[msdsit->first] = dDtemp;
//        cout<<"--------------------------------------"<<endl;
    }

//    system("pause");
    // current month:
    for(Map<String, deque<String> > ::iterator msdsit = calendarcat.begin(); msdsit != calendarcat.end(); msdsit++){
        dDtemp.clear();
//        cout<<"Name is "<<msdsit->first<<endl;
        for(int i = 0; i < cdate.size(); i++){

            cDay dtemp(cday[i],cdate[i],month, msdsit->second[i + 7],0);
            dtemp.setExpectAttr();
            dDtemp.push_back(dtemp);

//            cout<<dtemp<<endl;
        }
        base[msdsit->first] += dDtemp;
//        cout<<"--------------------------------------"<<endl;
    }
//    for(Map<String, deque<cDay> > ::iterator msdDit = base.begin(); msdDit != base.end(); msdDit++){
//        cout<<"name : "<<msdDit->first<<endl;
//        for(int i(0); i < msdDit->second.size(); i++){
//            cout<<(msdDit->second)[i]<<endl;
//        }
//    }

}

String Schedule::Data(int head = 0, int tail = 39) {
    tail += 7;
    if(tail >= base.begin()->second.size()){
        tail = base.begin()->second.size();
    }
    head += 7;
    if(head > tail){
        swap(head,tail);
    }

    outputManager om;
    deque<cDay> d = base.begin()->second;

    deque<String> dt;
    String date = "Date";
    dt.push_back(date);
    for(int i = head; i < tail; i++){
        dt.push_back(to_String(d[i].Date()));
    }

    deque<String> dy;
    String day = "Day";
    dy.push_back(day);
    for(int i = head; i < tail; i++){
        dy.push_back(d[i].Day());
    }

    deque<deque<String> > content;
    for(Map<String, deque<cDay> >::iterator msdit = base.begin(); msdit != base.end(); msdit++){
        deque<String> c;
        c.push_back(msdit->first);
        for(int i = head ; i < tail; i++){
            c.push_back((msdit->second)[i].Attr());
        }
        content.push_back(c);
    }

    return  om.createform(dt,dy,content);
}

void Schedule::loadLabor(Labor & l) {
    deque<cDay *> ddp;
    for(int i = 0 ; i < base[l.Name()].size(); i++){
        ddp.push_back(&((base[l.Name()])[i]));
    }
    l.loadSchedule(ddp);
}

ostream & operator<<(ostream &out, Schedule & d){
return out << d.Data();
}

#endif

//#define __SCHEDULE_CPP_UNIT_TEST__
#ifdef __SCHEDULE_CPP_UNIT_TEST__
int main(){
    Schedule s(5);
    cout<<s<<endl;
}
//
#endif