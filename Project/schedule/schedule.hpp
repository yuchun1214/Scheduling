
#ifndef __SCHEDULE_HPP__
#define __SCHEDULE_HPP__

#include "../DS/DS.cpp"
#include "../Day/Day.cpp"
#include "../labor/labor.cpp"
#include "../outputManager/output.cpp"
#include "../schedule_base/schedule_base.cpp"

#include <fstream>

using namespace DS;

class Schedule:protected __Schedule_base__{
private:
    Map<String, deque<cDay> > base;
    deque<cDay> holiday;
private:
    void virtualfunction(){}
    int month;
//
//    /**
//     * Load the file_name File and generate the deque<String> type variables
//     * @param file_name The name of file
//     * @return lines
//     */
//    deque<String> load(string file_name);
//
//    /**
//     * Split the lines to generate the content of schedule
//     * Function may return the 2-D array object
//     *
//     * @param lines load from the csv file
//     * @return lines content
//     */
//    deque<deque<String> > linesSplit(deque<String> lines);

    /**
     * create schedule data structure
     *
     * @param calendarContent
     * @param weightContent
     * @param lastContent
     */
    void createScheduleDataStructure(deque<deque<String> > calendarContent, deque<deque<String> > weightContent,
                                     deque<deque<String> > lastContent, deque<deque<String> > nextContent);

    /**
     * To modify the last schedule content for the last 7 day
     *
     * @param lastContent
     */
    void lastCalendarProcess(deque<deque<String> > & lastContent);

    void nextCalendarProcess(deque<deque<String> > & nextContent);

    void holidayProcess(deque<deque<String> > holiday);

    /**
     *
     * @param last
     * @param now
     * @return string map to series of day info
     */
    Map<String, deque<String> >
    calendarConcat(deque<deque<String> > last, deque<deque<String> > now, deque<deque<String> > next);

    /**
     * create the base
     *
     * @param calendarcat
     * @param cdate
     * @param cday
     * @param ldate
     * @param lday
     */
    void createBase(Map<String, deque<String> > calendarcat, Map<String, deque<String> > dayWeights,
                    deque<String> cdate, deque<String> cday, deque<String> ldate, deque<String> lday,
                    deque<String> ndate, deque<String> nday);


    Map<String, deque<String> > weightProcess(deque<deque<String> > weightContent);
public:
    Schedule();
    Schedule(int mon);
    String Data(int head, int tail);
    void loadLabor(Labor & l);

};

ostream & operator<<(ostream &out, Schedule & d) ;

#endif