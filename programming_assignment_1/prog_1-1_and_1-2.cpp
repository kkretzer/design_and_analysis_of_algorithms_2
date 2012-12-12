#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

typedef struct {
    long weight;
    long length;
} job_t;

bool diff_sort(job_t job1, job_t job2) { 
    long job1_cost = job1.weight - job1.length;
    long job2_cost = job2.weight - job2.length;
    if (job1_cost == job2_cost) {
        return job1.weight > job2.weight;
    }
    return job1_cost > job2_cost;
}

bool ratio_sort(job_t job1, job_t job2) {
    double job1_cost = ((double)job1.weight)/(double)job1.length;
    double job2_cost = ((double)job2.weight)/(double)job2.length;
    return job1_cost > job2_cost;
}

int main()
{
    long num_records, w, l, total_cost = 0, running_length = 0;
    std::vector<job_t> jobs;

    std::ifstream infile("jobs.txt");

    infile >> num_records;
    while (infile >> w >> l) {
        job_t j;
        j.weight = w;
        j.length = l;
        jobs.push_back(j);
    }

    sort(jobs.begin(), jobs.end(), diff_sort);
    //sort(jobs.begin(), jobs.end(), ratio_sort);

    for (const auto job : jobs) {
        running_length += job.length;
        total_cost += running_length * job.weight;
    }

    std::cout << "Total cost is " << total_cost << std::endl;
}
