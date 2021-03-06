#include "solution.hxx"


double get_number_res_from_order(char* order) {
    FILE* fp = NULL;
    double ret = 0;
    char buf[100];

    // call cmd in terminal
    fp = popen(order, "r");
    if (!fp) {
        printf("error in poen");
        return -1;
    }

    // get cmd output
    memset(buf, 0, sizeof(buf));
    fgets(buf, sizeof(buf)-1, fp);

    // parse double number
    ret = atof(buf);

    pclose(fp);
    return ret;
}

void SolutionInit(uint64_t *init_congestion_window, uint64_t *init_pacing_rate) {
    your_parameter["max_packet_size"] = 1350;
    your_parameter["init_ssthresh"] = 2 * your_parameter["max_packet_size"];
    your_parameter["last_time"]=0;
    your_parameter["ssthresh"] = your_parameter["init_ssthresh"];

    cout << "call python" << endl;
    system("python3 ./demo/hello_python3.py 0");

    cout << "python result : " << get_number_res_from_order("python3 ./demo/hello_python3.py 1 1 999") << endl;

    cout << "python result : " << get_number_res_from_order("python3 ./demo/hello_python3.py 2 my_model.pkl") << endl;
    cout << "over init" << endl;
}

uint64_t SolutionSelectBlock(Block* blocks, uint64_t block_num, uint64_t next_packet_id, uint64_t current_time) {
    /************** START CODE HERE ***************/
    // return the id of block you want to send, for example:
    uint64_t last_time = your_parameter["last_time"];
    // fprintf(stderr,"last_time = %lu, current_time = %lu\n", last_time, current_time);
    
    your_parameter["last_time"] = current_time;
    return blocks[0].block_id;
    /************** END CODE HERE ***************/
}

void SolutionCcTrigger(CcInfo *cc_infos, uint64_t cc_num, uint64_t *congestion_window, uint64_t *pacing_rate) {
    /************** START CODE HERE ***************/
    uint64_t cwnd = *congestion_window;
    for(uint64_t i=0;i<cc_num;i++){
        char event_type = cc_infos[i].event_type;
        // fprintf(stderr, "event_type=%c\n", event_type);
        const uint64_t max_packet_size = 1350;
        const uint64_t init_ssthresh = 2 * max_packet_size;
        if (your_parameter.count("ssthresh") <= 0)
            your_parameter["ssthresh"] = init_ssthresh;
        // return new cwnd, for example:
        uint64_t ssthresh = your_parameter["ssthresh"];
        if (event_type == 'F') {  // EVENT_TYPE_FINISHED
            if (cwnd > ssthresh)
                cwnd += max_packet_size / 2;
            else {
                cwnd += max_packet_size;
            }
        }
        if (event_type == 'D') {
            cwnd = cwnd / 2;  // EVENT_TYPE_DROP
            your_parameter["ssthresh"] = cwnd;
        }
        // fprintf(stderr,"new cwnd: %lu, ssthresh = %lu\n", cwnd, your_parameter["ssthresh"]);
    }
    // fprintf(stderr,"new cwnd: %lu, ssthresh = %lu\n", cwnd, your_parameter["ssthresh"]);
    *pacing_rate = 123456789;
    *congestion_window = cwnd;
    /************** END CODE HERE ***************/
}