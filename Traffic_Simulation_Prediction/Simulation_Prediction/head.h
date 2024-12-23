
#ifndef INC_1ST_WORK_HEAD_H
#define INC_1ST_WORK_HEAD_H

#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include <set>

#include<iostream>
#include<fstream>
#include<math.h>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <ctime>
#include <functional>
#include <utility>
#include <string>
#include <algorithm>
#include <map>
#include <thread>
#include <future>
#include <boost/thread/thread.hpp>
#include <semaphore.h>
#include "boost/thread.hpp"
#include <time.h>
#include <cstdlib>
#include <random>

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <set>
#include <sstream>
#define INF 999999999

using namespace std;

// Parallel Computation

class Semaphore
{
public:
    Semaphore (int count_ = 0)
            : count(count_) {}

    inline void Signal()
    {
        unique_lock<std::mutex> lock(mtx);
        count++;
        cv.notify_one();
    }

    inline void Wait()
    {
        unique_lock<std::mutex> lock(mtx);

        while(count == 0)
        {
            cv.wait(lock);

        }
        count--;
    }


    mutex mtx;
    condition_variable cv;
    int count;
};

/*class Semaphore {
public:
    explicit Semaphore(int count = 0) : count_(count) {
    }

    void Signal() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    void Wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [=] { return count_ > 0; });
        --count_;
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};*/

// 定义一个用作词典键的结构体
struct RoadKey {
    int lane_num;
    float speed_limit;
    float edge_length;
    // char turn_direction;
    int driving_number;
    int delay_time;
    int lowSpee_time;
    int wait_time;
    int ratio;
    int length_square;

    // 定义比较操作符，以便在词典中使用这个结构体作为键
    bool operator==(const RoadKey &other) const {
        return std::tie(lane_num, speed_limit, edge_length, driving_number, delay_time, lowSpee_time, wait_time, ratio, length_square) ==
               std::tie(other.lane_num, other.speed_limit, other.edge_length, other.driving_number, other.delay_time, other.lowSpee_time,other.wait_time, other.ratio, other.length_square);
    }
};

#include <functional>

namespace std {
    template <>
    struct hash<RoadKey> {
        std::size_t operator()(const RoadKey& k) const {
            size_t res = 17;  // 初始化一个基值
            // res = res * 31 + hash<string>()(k.edge_str);       // 对 edge_str 进行哈希
            res = res * 31 + hash<int>()(k.lane_num);            // 对 lane_num 进行哈希
            res = res * 31 + hash<float>()(k.speed_limit);       // 对 speed_limit 进行哈希 (float)
            res = res * 31 + hash<float>()(k.edge_length);       // 对 edge_length 进行哈希 (float)
            res = res * 31 + hash<int>()(k.driving_number);      // 对 driving_number 进行哈希
            res = res * 31 + hash<int>()(k.delay_time);          // 对 delay_time 进行哈希
            res = res * 31 + hash<int>()(k.lowSpee_time);        // 对 lowSpee_time 进行哈希
            res = res * 31 + hash<int>()(k.wait_time);           // 对 wait_time 进行哈希
            res = res * 31 + hash<int>()(k.ratio);               // 对 ratio 进行哈希
            res = res * 31 + hash<int>()(k.length_square);       // 对 length_square 进行哈希
            return res;  // 返回组合后的哈希值
        }
    };
}




//Contain Road Information
typedef struct ROAD
{
    int roadID;
    int ID1, ID2;
    int length;
    int travelTime;
    int direction;
    int speedLimit;
    int laneNum;
    int width;
    int kindNumber;
    string kind;
}Road;

//Contain Road Information
typedef struct ROADMORE
{
    int nodeID1;
    int nodeID2;
    int routeID;
    int length;
    int direction;
    int speedLimit;
    int laneNum;
    int width;
    int kindNumber;
    string kind;
}roadMore;

// Structure to store edge's static infor
struct EdgeInfo {
    int lane_num;
    float speed;
    float length;
    string edge_str;
};

class Graph {
public:

    // Step 1: Data Cleaning
    // -----------------------------------------------------------------------------

    // Check Why "BJ" had different edge number with "Minimal Travel Time"
    void edge_num_check();
    Semaphore* smGlobal = new Semaphore(1);

    // Step 2: Data Preparation
    // -----------------------------------------------------------------------------

    // Define Data Path
    string Base = "/Users/xuzizhuo/Desktop/Main Folder/My_works/Traffic_Simulation_Work/Traffic_Simulation_Prediction/Simulation_Prediction/Manhattan_Data/";
    // Read Road Network
    void read_graph();       // Can Be "one-way" (edge:651749) or "two-way" (edge:774660)
    // string BJ_NodeWeight = Base + "BJ_NodeWeight";
    string BJ = Base + "Manhattan_network_BJ.txt";                // ID1, ID2, Weight (Length in Meters)
    int nodenum;                                    // Node Number (296710)
    int edgenum;                                    // Edge Number (774660 or 651748) 387587
    vector<vector<pair<int, float>>> graphLength;      // ID1, ID2, Length
    vector<vector<pair<int, int>>> graphRoadID;     // ID1, ID2, RoadID
    vector<Road> roadInfor;                         // Road Info (ID1, ID2, RoadID, Length, Time
    vector<set<int>> adjNodes;                      // ID1, ID2
    map<pair<int, int>, int> nodeID2RoadID;         // <ID1, ID2> -> RoadID
    map<int, pair<int, int>> roadID2NodeID;         // RoadID -> <ID1, ID2>
    map<pair<int, int>, int> nodeID2minTime;        // <ID1, ID2> -> Min Travel Time
    string BJ_minTravleTime = Base +                // ID1, ID2, Weight (Min Travel Time in s)
                              "Manhattan_network_min_Travel_Time.txt";
    vector<vector<pair<int,int>>> graphTime;        // ID1, ID2, Weight (Min Travel Time)
    // Read Road Information
    void read_road_info();
    vector<roadMore> roadInforMore;
    string beijingMoreRoadInfo = Base + "beijingMoreRoadInfo";

    // Read query, route, time data
    vector<vector<int>> read_query(string filename, int num);
    string queryPath = Base + "query.txt";
    vector<vector<int>> queryDataRaw;
    vector<vector<int>> read_route(string filename, int num);
    string route_path = Base + "route.txt";
    vector<vector<int>> routeDataRaw;
    unordered_map<int, unordered_map<int, vector<int>>> route_time_Dict;
    vector<vector<int>> read_time(string filename, int num, vector<vector<int>> query);
    string time_path = Base + "time.txt";
    vector<vector<int>> timeDataRaw;
    tuple<vector<int>, int> read_time_no_wait(string filename, int num);
    string time_path_no_wait = Base + "time_no_wait.txt";
    vector<int> time_no_wait;
    // Remove data with duplicate values
    void removeDuplicates();
    // Check if route data, query data, and time data size are same
    void check_size();
    // Cut route data
    vector<vector<int>> cut_route_data(vector<vector<int>> &routeDataRaw, int avg_length);
    // Cut query data
    vector<vector<int>> cut_query_data(vector<vector<int>> &queryDataRaw, vector<vector<int>> &routeData, int avg_length);
    // Cut time data
    vector<vector<int>> cut_time_data(vector<vector<int>> &timeDataRaw, int avg_length);

    // Split Route and Query Data as Average Length
    pair<vector<vector<int>>, vector<vector<int>>> data_length_modify(vector<vector<int>> &queryDataRaw,
                                                                      vector<vector<int>> &routeDataRaw, int avg_length);
    // Convert Route from "Node ID Pair" to "Road ID"
    void route_nodeID_2_roadID(vector<vector<int>> &routeData);
    // Convert Single Route from "Node ID Pair" to "Road ID"
    void route_nodeID_2_roadID_single(vector<int> &routeData);
    vector<vector<int>> routeRoadID;
    // Classify Each Road with A Unique Latency Function
    vector<vector<pair<int,vector<pair<int,int>>>>> timeRange;
    // int realPercent;
    void classify_latency_function();
    // Generate Hour Index and Its Related Minutes Index
    pair<int, int> time_to_base_index(int seconds, int minRange);
    // Define Flow Base
    void flow_base_ini(int minRange, int flowValue);
    vector<vector<pair<int, vector<vector<int>>>>> flowBaseList;
    int minRange;   // minRange -> Min Time Range
    int flowIni;    // flowIni -> Initialized Flow Base Value

    // Step 3: ALGORITHM I SIMULATION
    // -----------------------------------------------------------------------------

    vector<int> Dij_vetex(int ID1, int ID2);
    // flow -> travel time range -> travel time
    int flow2time_by_range(int &ID1index, int &ID2index, int &flow);
    //
    void read_edge_feature_2_map(const string& filePath);
    string edge_id_to_features_path = Base + "edge_id_to_features.csv";

    map<int, EdgeInfo> edge_id_to_features;

    //
    void readConnectionsToDirections(const std::string& filename);
    map<pair<int, int>, char> connections_to_direction;
    string connections_to_direction_path = Base + "connections_to_directions.csv";

    // 定义函数，返回给定边的下一边的方向
    char findNextEdgeDirection(int currentEdgeID, int route_id);
    //
    set<char> processVehicleDirections(const std::vector<int>& vehicleIDs, int& currentEdgeID);
    //
    void buildDictionary(const std::string& filename);
    unordered_map<RoadKey, double> dictionary;
    string model_catch_dic_path = Base + "model_catching_with_travel_time_1.txt";
    // Simulation algorithm
    vector<vector<pair<int, float>>> alg1Records(
            vector<vector<int>> &Q, vector<vector<int>> &Pi,
            bool range, bool server, bool catching, bool write, bool latency, string te_choose);
    string model_catching = Base + "model_catching.txt";
    vector<vector<pair<int, float>>> ETA_result;
    vector<vector<vector<int>>> nodes_label; //PathID, Node Index, Arrived Time
    vector<vector<pair<int, map<float, vector<vector<int>>>>>> timeFlowChange;
    float sigma = 0.15; float varphi = 20; float beta = 2;
    float percent;
    int small, big;
    // min travel time x 0.15 * (flow/20)^2
    int minTravel;
    int realTravel;
    int catching_no_found;
    int catching_found;

    // Estimate average travel time of ground truth
    float AVG_estimation(vector<vector<int>> routeData, vector<vector<int>> timeData);
    // Estimate travel time MSE between simulated results and truth
    float MSE_estimation(vector<vector<int>> time, vector<vector<pair<int, float>>> ETA);
    // Find previous or next edge of current one.
    int find_direction(vector<vector<int>> &routeData, vector<vector<int>> &routeDataEdge,
                       int routeID, int nodeStart, int nodeEnd);
    // Export record information on each edge for further analysis
    void export_time_records(string route_num, vector<vector<int>> &routeData,
                             vector<vector<int>> &routeDataEdge, vector<vector<int>> &timeData,
                             vector<vector<pair<int,int>>> &ETA);
    string file_path_time_records = Base + "time_records_out.txt";

    void Traffic_Prediction(vector<vector<pair<int, float>>> ETA_result);

    // Step 4: ROUTE DATA UPDATE OPERATIONS PREPARATION
    // -----------------------------------------------------------------------------

    // Check Simulation
    void check_simulation(int simulation);
    // Change NodeID to RoadID
    void nodeID_2_roadID_in_records(vector<vector<pair<int,map<float, vector<vector<int>>>>>> &timeFlowChange);
    vector<map<float, vector<vector<int>>>> route_timeFlowChange;
    // Check Correctness of Time Records in Slices
    void time_record_correct_check();
    // Find Min Departure Time from Queries
    void min_depar_time(vector<vector<int>> &Q);
    int minDeparture; int minHour;
    // Convert int Time into UTC Hour
    int time_2_hour(int intTime);
    // Convert Hour to Its Belonged Index
    int hour_2_index(int hour);
    // Split Time Flow Change into Time Slices
    void split_2_time_slices(vector<map<float, vector<vector<int>>>> &route_timeFlowChange);
    vector<vector<map<int, vector<vector<int>>>>> timeSlice; // RoadSegmentID: TimeSlice: TimeRecord
    // Convert Node Constructed Route to Road Constructed Route
    vector<int> route_node_2_route_road(vector<int> &routeNode);
    // Generate New Data for Insertion
    void data_generation(string route_file, string depar_file, string Pi_file, int newNum, int avg_length, bool cut);
    string routeRoadPath = Base + "new_data_update/test_route";
    string deparTimePath = Base + "new_data_update/test_depar";
    string routeNodePath = Base + "new_data_update/test_Pi";
    // Generate Data Same as Simulation Operation for Comparison
    void data_generation_same_simulation(
            string route_file, string depar_file, string Pi_file, vector<vector<int>> &query_data, vector<vector<int>> &route_data);
    // Read Generated Route Data
    void read_new_data(string routeRoadPath, string deparTimePath, string routeNodePath);
    vector<vector<int>> Pi;
    vector<pair<int, vector<int>>> routeRoadInput;
    vector<int> departTimeList;
    int routeDataSize;
    // "ETA_result" Initialization
    void ETA_initialization(bool simulation, bool print);

    // Step 5: ROUTE DATA INSERTION OPERATIONS
    // -----------------------------------------------------------------------------

    // Insertion Operation (Main)
    void update_operation_insertion(bool paralle, bool terminal, bool range, bool print);
    vector<vector<map<int, vector<vector<int>>>>> timeSliceInsert;
    //  Insertion Operation by Inserting New Route Data One by One
    vector<int> one_route_update_insertion(pair<int, vector<int>> &newRoute, int inTime, bool parallel, bool terminal, bool range, bool print);
    //  Insertion Operation for Different Road Has Different Operation
    //  For a Road ID Constructed Route, Find Next Road ID
    int find_next_roadID(vector<int> route, int roadSegmentID);
    //  Check If Two Traffic Flows Are In Same Range
    bool flow_same_range_check(int &ID1index, int &ID2index, int &flow1, int &flow2);
    //  This Is Insertion Operation for The First Road of New Route
    vector<pair<pair<int,int>, tuple<int, map<int, vector<vector<int>>>, vector<int>>>> update_operation_1st(
            int &RoadSegmentID, int &inTime, pair<int, vector<int>> &newRoutePair, bool &parallel, bool &terminal, bool &range, bool print);
    //  This Is Insertion Operation for The Further Road of New Route or Propagated Road
    vector<pair<pair<int,int>, tuple<int, map<int, vector<vector<int>>>, vector<int>>>> updateOperationFurther(
            pair<int,int> &RoadSegmentID, int &inTime, pair<int, vector<int>> &newRoutePair, pair<int, vector<int>> &routeProp,
            map<int, vector<vector<int>>> &InsertPre, vector<int> &DeletionPre, bool &parallel, bool &terminal, bool &range, bool print);
    // Insertion Operation Parallel
    void update_operation_parallel(vector<pair<pair<int, vector<int>>, int>> &newDataMulti, bool &parallel, bool terminal,
                                   bool &range, bool print, int &threadNum);
    //  Split New Route Data into Groups
    void multi_new_data_initial(void);
    vector<pair<pair<int, vector<int>>, int>> newDataMulti;
    //  Insertion Operation by Inserting New Route Data One by One (Parallel Version)
    void one_route_parallel_update(vector<pair<pair<int, vector<int>>, int>> &multi_test,
                                   int i, int j, vector<Semaphore*>& vLock, bool parallel, bool terminal, bool range, bool print);
    vector<Semaphore*> vLock;
    vector<int> affectedRoadParallel;

    // Step 6: TRAVEL TIME ESTIMATION (ETA) UPDATE
    // -----------------------------------------------------------------------------
    void ETA_update();

    // Step 7: ROUTE DATA DELETION OPERATIONS
    // -----------------------------------------------------------------------------

    // Generate New Data for Deletion
    // Main Idea: Capturing Existing Route as New Deletion Data
    void dele_data_generation(string route_file, string Pi_file, string route_index_file, int newNum);
    string routeRoadPathD = Base + "new_data_update/deletion_routeNode";
    string routeNodePathD = Base + "new_data_update/deletion_routeRoad";
    string routeIndexpath = Base + "new_data_update/deletion_routeRoadIndex";
    // Read Selected Deleted Route Data
    void read_deletion_data(string routeRoadPath, string routeRoadIndex);
    vector<pair<int, vector<int>>> routeRoadDelInput;
    // Deletion Operation
    void update_operation_deletion(bool parallel, bool terminal, bool range, bool print);
    vector<vector<map<int, vector<vector<int>>>>> timeSliceDel;
    vector<int> one_route_update_deletion(pair<int, vector<int>> delRoute, bool parallel, bool terminal, bool range, bool print);
    vector<pair<pair<int,int>, tuple<int, map<int, vector<vector<int>>>, vector<int>>>> deletion_operation_1st(
            int &RoadSegmentID, int &inTime, pair<int, vector<int>> &newRoutePair, bool &parallel, bool &terminal, bool &range, bool print);
    //  This Is Deletion Operation for The Further Road of New Route or Propagated Road
    vector<pair<pair<int,int>, tuple<int, map<int, vector<vector<int>>>, vector<int>>>> update_operation_deletion_further(
            pair<int,int> &RoadSegmentID, int &inTime, pair<int, vector<int>> &newRoutePair, pair<int, vector<int>> &routeProp,
            map<int, vector<vector<int>>> &InsertPre, vector<int> &DeletionPre, bool &parallel, bool &terminal, bool &range, bool print);
    //  Split New Route Data into Groups
    void multi_del_data_initial(void);
    vector<pair<int, vector<int>>> delDataMulti;
    //  Deletion Operation by Deleting Target Route Data One by One (Parallel Version)
    void del_operation_parallel(vector<pair<int, vector<int>>> &delDataMulti,
                                bool &parallel, bool terminal, bool &range,  bool print, int &threadNum);
    //  Deletion Operation by Inserting New Route Data One by One (Parallel Version)
    void one_route_parallel_update_del(vector<pair<int, vector<int>>> &multi_test,
                                       int i, int j, vector<Semaphore*>& vLock, bool parallel, bool terminal, bool range, bool print);
    vector<int> affectedRoadParallelDel;
    vector<Semaphore*> vLockDel;
    void stat();

    // Appendix
    // -----------------------------------------------------------------------------
    // Count Number of Lines in ".txt" File
    int CountLines(string filename){
        ifstream ReadFile;
        int n=0;
        char line[512];
        string temp;
        ReadFile.open(filename,ios::in);
        if(ReadFile.fail())
            return 0;
        else
        {
            while(getline(ReadFile,temp))
            {
                n++;
            }
            return n;
        }
        ReadFile.close();
    }
    // Randomly Generate Unordered Integer
    vector<int> randperm(int Num)
    {
        vector<int> temp;
        for (int i = 0; i < Num; ++i)
        {
            temp.push_back(i);
        }
        std::random_device rd;
        std::mt19937 g(rd());
        shuffle(temp.begin(), temp.end(),g);
        return temp;
    }

    // Abundant Functions
    // -----------------------------------------------------------------------------

    //Classify Traffic Flow's Range
    void flow_range_classification(float constant, int power, int maxTime);
    vector<int> range;
};


namespace benchmark {

#define NULLINDEX 0xFFFFFFFF

    template<int log_k, typename k_t, typename id_t>
    class heap {

    public:

        // Expose types.
        typedef k_t key_t;
        typedef id_t node_t;

        // Some constants regarding the elements.
        //static const node_t NULLINDEX = 0xFFFFFFFF;
        static const node_t k = 1 << log_k;

        // A struct defining a heap element.
        struct element_t {
            key_t key;
            node_t element;

            element_t() : key(0), element(0) {}

            element_t(const key_t k, const node_t e) : key(k), element(e) {}
        };


    public:

        // Constructor of the heap.
        heap(node_t n) : n(0), max_n(n), elements(n), position(n, NULLINDEX) {
        }

        heap() {

        }

        // Size of the heap.
        inline node_t size() const {
            return n;
        }

        // Heap empty?
        inline bool empty() const {
            return size() == 0;
        }

        // Extract min element.
        inline void extract_min(node_t &element, key_t &key) {
            assert(!empty());

            element_t &front = elements[0];

            // Assign element and key.
            element = front.element;
            key = front.key;

            // Replace elements[0] by last element.
            position[element] = NULLINDEX;
            --n;
            if (!empty()) {
                front = elements[n];
                position[front.element] = 0;
                sift_down(0);
            }
        }

        inline key_t top() {
            assert(!empty());

            element_t &front = elements[0];

            return front.key;

        }

        inline node_t top_value() {

            assert(!empty());

            element_t &front = elements[0];

            return front.element;
        }

        // Update an element of the heap.
        inline void update(const node_t element, const key_t key) {
            if (position[element] == NULLINDEX) {
                element_t &back = elements[n];
                back.key = key;
                back.element = element;
                position[element] = n;
                sift_up(n++);
            } else {
                node_t el_pos = position[element];
                element_t &el = elements[el_pos];
                if (key > el.key) {
                    el.key = key;
                    sift_down(el_pos);
                } else {
                    el.key = key;
                    sift_up(el_pos);
                }
            }
        }


        // Clear the heap.
        inline void clear() {
            for (node_t i = 0; i < n; ++i) {
                position[elements[i].element] = NULLINDEX;
            }
            n = 0;
        }

        // Cheaper clear.
        inline void clear(node_t v) {
            position[v] = NULLINDEX;
        }

        inline void clear_n() {
            n = 0;
        }


        // Test whether an element is contained in the heap.
        inline bool contains(const node_t element) const {
            return position[element] != NULLINDEX;
        }


    protected:

        // Sift up an element.
        inline void sift_up(node_t i) {
            assert(i < n);
            node_t cur_i = i;
            while (cur_i > 0) {
                node_t parent_i = (cur_i - 1) >> log_k;
                if (elements[parent_i].key > elements[cur_i].key)
                    swap(cur_i, parent_i);
                else
                    break;
                cur_i = parent_i;
            }
        }

        // Sift down an element.
        inline void sift_down(node_t i) {
            assert(i < n);

            while (true) {
                node_t min_ind = i;
                key_t min_key = elements[i].key;

                node_t child_ind_l = (i << log_k) + 1;
                node_t child_ind_u = std::min(child_ind_l + k, n);

                for (node_t j = child_ind_l; j < child_ind_u; ++j) {
                    if (elements[j].key < min_key) {
                        min_ind = j;
                        min_key = elements[j].key;
                    }
                }

                // Exchange?
                if (min_ind != i) {
                    swap(i, min_ind);
                    i = min_ind;
                } else {
                    break;
                }
            }
        }

        // Swap two elements in the heap.
        inline void swap(const node_t i, const node_t j) {
            element_t &el_i = elements[i];
            element_t &el_j = elements[j];

            // Exchange positions
            position[el_i.element] = j;
            position[el_j.element] = i;

            // Exchange elements
            element_t temp = el_i;
            el_i = el_j;
            el_j = temp;
        }


    private:

        // Number of elements in the heap.
        node_t n;

        // Number of maximal elements.
        node_t max_n;

        // Array of length heap_elements.
        vector<element_t> elements;

        // An array of positions for all elements.
        vector<node_t> position;
    };
}

#endif //INC_1ST_WORK_HEAD_H
