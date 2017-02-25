/*
 * prog9_acd21.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: Adam C. De Leon
 *      Description: This program's intended use is for helping Fruit Pickers,
 *      Inc. maintain its labor force of fruit pickers. Many growers are
 *      willing to pay more for pickers who work quickly and efficiently. To
 *      help determine their best pickers, Fruit Pickers, Inc. holds a contest
 *      once a year where pickers are sent out to selected orchards for a week
 *      (7 days). Each picker is assigned a 4-digit ID number. The weight of
 *      the fruit that they pick each day is recorded in a text
 *      file(contest_data.txt) that contains on each line the picker ID and
 *      weight of the fruit picked for each day of the week of the contest.
 *      The program reads the input file to EOF and stores the data in two
 *      parallel arrays. One for the picker ID number and a 2-D array for
 *      his/her daily picking weights. The program then produces an output
 *      file (prog9_out_acd21.txt), in which Picker ID, Total weight picked
 *      over the week, and Average weight picked/day; appear listed in that
 *      order, as a header for the corresponding data that follows. The output
 *      file then displays the picker ID of the picker with the highest
 *      average wgt/day, the total weight picked by all pickers for each day
 *      of the contest, Picker ID of the picker who picked the most fruit on a
 *      day during the week of the contest and the weight that they picked.
 *      Fruit Pickers, Inc. employs a maximum of 80 employees and the constest
 *      is only 7 days long, which are both indicated as global constants in
 *      the program.
 *
 *      Return 1: Error code; displays that the input file could not be found.
 */
# include <iostream>
# include <fstream>
# include <iomanip>

using namespace std;
//Global Constants
const int MAX_PICKERS = 80;
const int DAYS = 7;

//Prototypes for functions.
int GetContestInfo(int [], double [][DAYS]);

double PickerWeekTotal(double [][DAYS], int);

double DailyTotal(double [][DAYS], int, int);

int MostPickedDay(double [][DAYS], int, int &);

int main() {
    int picker_id[MAX_PICKERS];     //Array for picker's ID number.
    double weight[MAX_PICKERS][DAYS]; //Array for daily picking weights.
    double picker_week_total;      //Variable declarations.
    double day_total;
    double avg_total;
    double most_weight;
    int num_pickers;
    int picker_index;
    int day_num;
    int tally;
    int most_picked;

    ofstream fout;                 //Output file object
    fout.open("prog9_out_acd21.txt");  //Open the output file.

    //Retrieving the number of pickers in the input file.
    num_pickers = GetContestInfo(picker_id, weight);

    //Validation of successful input file opening.
    if (num_pickers == -1) {
        cout << "The input file could not be found." << endl;
        return 1;
    }

    //Title for output file data.
    fout << setprecision(1) << fixed;
    fout << "Picker ID" << setw(15) << "Total Wgt/Wk" << setw(15)
         << "Avg Wgt/Day" << endl;
    fout << endl;

    //Initialization of necessary variables.
    most_weight = 0;
    tally = 0;
    avg_total = 0;

    //Reading info for each picker and each day.
    for (picker_index = 0; picker_index < num_pickers; picker_index++) {
        //Determines total weight picked for each picker.
        picker_week_total = PickerWeekTotal(weight, picker_index);

        //Calculates average weights/day.
        for (day_num = 0; day_num < DAYS; day_num++) {
            avg_total = avg_total + weight[picker_index][day_num];
        }
        avg_total = avg_total / DAYS;

        //Determines picker with highest average weight/day.
        if (most_weight < avg_total) {
            most_weight = avg_total;
            tally = picker_index;
        }

        //Display values.
        fout << picker_id[picker_index] << setw(14) << picker_week_total
             << " lbs" << setw(10) << avg_total << " lbs/day" << endl;

        //Reset average for next picker.
        avg_total = 0;
    }

    fout << endl;

    //Displays info for picker with highest daily average.
    fout << "Picker " << picker_id[tally] << " had the highest avg "
            "wgt/day of " << most_weight << " lbs" << endl;
    fout << endl;

    //Reading info for each day.
    for (day_num = 0; day_num < DAYS; day_num++) {
        //Determines total weight picked each day by all pickers.
        day_total = DailyTotal(weight, num_pickers, day_num);

        //Displays values for each day.
        fout << "Day # " << day_num << " total picked is " << day_total
             << " lbs" << endl;
    }
    fout << endl;

    //Determines on which day and by which picker the most weight was picked.
    most_picked = MostPickedDay(weight, num_pickers, day_num);

    //Displays results.
    fout << "Picker ID # " << picker_id[most_picked] << " picked "
         << weight[most_picked][day_num] << " lbs" << " on day # " << day_num
         << endl;

    //Close the output file.
    fout.close();
    return 0;
}

/*****************************************************************************
 * GetContestInfo - This function opens and read the data from the input file
 * and closes the file. it returns the actual number of pickers. If there is a
 * problem opening the input file, it returns a -1 as the number of pickers.
 */
int GetContestInfo(int picker_id[], double weight[][DAYS]) {
    int num_pickers;
    ifstream fin;
    fin.open("contest_data.txt");

    if (!fin) {
        num_pickers = -1;
    } else {
        num_pickers = 0;
        while (num_pickers < MAX_PICKERS && fin >> picker_id[num_pickers]) {
            for (int day_num = 0; day_num < DAYS; day_num++)
                fin >> weight[num_pickers][day_num];

            num_pickers = num_pickers + 1;
        }
    }
    fin.close();
    return num_pickers;
}

/*****************************************************************************
 * PickerWeekTotal - This function has as parameters the picking weights array
 * and the index of a picker. It calculates and returns the total weight
 * picked for the week for that picker.
 */
double PickerWeekTotal(double weight[][DAYS], int picker_index) {
    double one_wkly_total;
    one_wkly_total = 0;

    for (int day_num = 0; day_num < DAYS; day_num++)
        one_wkly_total = one_wkly_total + weight[picker_index][day_num];

    return one_wkly_total;
}

/*****************************************************************************
 * DailyTotal - This function has as its parameters the picking weights array,
 * and a day number. It calculates and returns the total weight of fruit
 * picked that day by all pickers.
 */
double DailyTotal(double weight[][DAYS], int num_of_pickers, int day_num) {
    double day_total;

    day_total = 0;
    for (int picker_index = 0; picker_index < num_of_pickers; picker_index++)
        day_total = day_total + weight[picker_index][day_num];

    return day_total;
}

/*****************************************************************************
 * MostPickedDay - This function has as parameters the picking weights array,
 * the number of pickers, and an integer that is passed by reference used to
 * return the day number on which the most fruit was picked in a single day.
 * The function returns the location in the weights array of the picker that
 * picked the most fruit in a single day.
 */
int MostPickedDay(double weight[][DAYS], int num_of_pickers, int &ref_day_num) {
    double most_picked;
    int location;

    most_picked = 0;
    for (int picker_index = 0; picker_index < num_of_pickers; picker_index++) {
        for (int day_num = 0; day_num < DAYS; day_num++) {
            if (most_picked < weight[picker_index][day_num]) {
                most_picked = weight[picker_index][day_num];
                location = picker_index;
                ref_day_num = day_num;

            }
        }
    }
    return location;
}

