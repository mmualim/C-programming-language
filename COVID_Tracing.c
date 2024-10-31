/* Solution to comp20005 Assignment 1, 2020 semester 2.

   Authorship Declaration:
   Name : Merlyvia Mualim
   Student Id : 1086238
   Due date : 4pm Wednesday, 30th September 2020 
   
   C programming is fun!*/

#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAXLINE 99
#define ID_LENGTH 9
#define PI 3.14159
#define DEGREE 180
#define HAVERSINE_CONSTANT 6371

/*Struct to store the file data*/
typedef struct {
    double visited_lat, visited_long, user_home_lat, user_home_long;
    int visit_start, visit_end;
    char user_ID[ID_LENGTH];
} data;

/*Function to read the file and putting it into an array*/
int load(data records[]) {
    int count = 0;
    while(scanf("%lf %lf %lf %lf %d %d %s\n", &records[count].visited_lat,
        &records[count].visited_long, &records[count].user_home_lat,
        &records[count].user_home_long, &records[count].visit_start,
        &records[count].visit_end, records[count].user_ID) != EOF) {
        count++;
    }
    return count;
}

/*Convert to radian*/
double toRadian(double distance_p1_p2) {
    return distance_p1_p2*(PI/DEGREE);
}

/*Calculate user distance from home using haversine formula*/
double dist(double lat1, double long1, double lat2, double long2) {
    double angle_distance, chord_length;
    double x = (lat1 - lat2);
    double y = (long1 - long2);

    chord_length = (sin(toRadian(x)/2) * sin(toRadian(x)/2)) +
                   (cos(toRadian(lat1)) * cos(toRadian(lat2)) *
                   sin(toRadian(y)/2) * sin(toRadian(y)/2));
    angle_distance = 2*atan2(sqrt(chord_length), sqrt(1 - chord_length));

    return HAVERSINE_CONSTANT*angle_distance;
}

/*Calculate each distance from visit location to home and put into array*/
void calculateHomeDistance(data records[], double distances[], int total) {
    int i;
    for(i=0; i < total; i++) {
        distances[i]= dist(records[i].visited_lat, records[i].visited_long,
                      records[i].user_home_lat, records[i].user_home_long);
    }
}

/*Calculate the overlap time of the user*/
int overlap(int start1, int end1, int start2, int end2) {
    int meetStart = 0, meetEnd = 0;
    if (start2>=start1 && start2<=end1) {
        meetStart=start2;
    }
    else if (start1>=start2 && start1<=end2) {
        meetStart = start1;
    }
    if(end2>=start1 && end2<=end1) {
        meetEnd=end2;
    }
    else if(end1>=start2&&end1<=end2) {
        meetEnd=end1;
    }
    return meetEnd-meetStart;
}

/*Calculate visited location distance of each user that is <2 meter*/
int calculateVisited(data records[], double distances[], int overlapTime[],
                      int total, int index[]) {
    int i, j;
    int count = 0;
    int maxOverlap=0;
    double thresholdDistance = 0.002; /*2 meters in km*/
    double thresholdTime = 900; /*15 minutes in seconds*/

    for(i=0; i < total; i++) {
        j = i + 1;
        while (j < total) {
            if (strcmp(records[j].user_ID, records[i].user_ID) == 0) {
                j++;
            }
            else {
                if ((dist(records[i].visited_lat, records[i].visited_long,
                    records[j].visited_lat, records[j].visited_long) 
                    < thresholdDistance) && (overlap(records[i].visit_start,
                    records[i].visit_end, records[j].visit_start,
                    records[j].visit_end) > thresholdTime)) {
                    if(overlap(records[i].visit_start, records[i].visit_end,
                               records[j].visit_start, records[j].visit_end)
                               > maxOverlap) {
                        distances[0] = dist(records[i].visited_lat, 
                                        records[i].visited_long,
                                        records[j].visited_lat, 
                                        records[j].visited_long);
                        overlapTime[0] = overlap(records[i].visit_start,
                                        records[i].visit_end, 
                                        records[j].visit_start,
                                        records[j].visit_end);
                        maxOverlap = overlapTime[0];
                        index[0] = i; /*store the index to find*/
                        index[0] = j; /*the location of the user ID*/
                        
                    }
                    count++;;
                }
                j++;
            }

        }
    }
    return count;
}

/*Stage 1*/
void Stage1(char user_ID[ID_LENGTH], double latitude, double longitude,
            int start_time, int end_time, double distance) {
    printf("Stage 1\n");
    printf("==========\n");
    printf("User: #%s\n", user_ID);
    printf("Visited location: <%.6f, %.6f>\n", latitude, longitude);
    printf("Start time: %05d\n", start_time);
    printf("End time: %05d\n", end_time);
    printf("Distance from home: %05.2f km\n", distance);
}

/*Stage 2*/
void Stage2(data records[], double distances[], int count) {
    int i;
    printf("\nStage 2\n");
    printf("==========\n");

    /*Visited location is computed and visualised*/
    for(i=0; i < count; i++) {
        int visual;
        printf("#%s", records[i].user_ID);
        printf(", distance from home: %05.2f |", distances[i]);
        for(visual=0; visual < distances[i]; visual++) {
            if((visual+1) % 5 != 0) {
                printf("-");
            }
            else printf("+");
        }
        printf("\n");
    }
}

/*Stage 3 desperate attempt*/
void Stage3(data records[], double visitedDistance[], int overlapTime[], 
            int pairs, int index[]) {
    int i = index [0];
    int j = index [1];
    printf("\nStage 3\n");
    printf("==========\n");
    printf("Number of close-contact pairs: %d\n", pairs);
    printf("Longest overlap time: %05d seconds\n", overlapTime[0]);
    printf("#%s @ <%.6f ,%.6f>\n", records[i].user_ID, records[i].visited_lat,
          records[i].visited_long);
    printf("#%s @ <%.6f ,%.6f>\n", records[j].user_ID, records[j].visited_lat,
          records[j].visited_long);
}

int main(int argc, char *argv[]) {
    data records[MAXLINE];
    int count = load(records);

    double homeDistances[MAXLINE];
    calculateHomeDistance(records, homeDistances, count);

    double visitedDistances[MAXLINE];
    int overlapTime[MAXLINE];
    int index[3];
    int pairs = calculateVisited(records, visitedDistances, overlapTime, count,
                                index);

    /*Stage 1 run*/
    Stage1(records[0].user_ID, records[0].visited_lat, records[0].visited_long,
           records[0].visit_start, records[0].visit_end, homeDistances[0]);
    
    /*Stage 2 run*/
    Stage2(records, homeDistances, count);

    /*Stage 3 run*/
    Stage3(records, visitedDistances, overlapTime, pairs, index);

    return 0;
}
