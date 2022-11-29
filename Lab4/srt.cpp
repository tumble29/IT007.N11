/*######################################
/*# University of Information Technology #
# IT007 Operating System #
# IT007.N11 - WeKids #
# File: srt.cpp #
######################################*/

#include<iostream>
#include<vector>
#include<algorithm>
#include<stdio.h>
using namespace std;

class Process {
public:
	int pn, arr, bur, star, finish, tat, wt;
	Process(int pn, int arr, int bur, int star = -1, int finish = 0, int tat = 0, int wt = 0) {
		this->pn = pn;
		this->arr = arr;
		this->bur = bur;
		this->star = star;
		this->finish = finish;
		this->tat = tat;
		this->wt = wt;
	};
};
class ReadyProcess {
public:
	int pn, bur;
	ReadyProcess(int i, Process p) { pn = i; this->bur = p.bur; }
	void swap_process(ReadyProcess& p) { swap(pn, p.pn); swap(bur, p.bur); }
};
class NewProcess {
public:
	int pn, arr;
	NewProcess(int i, Process p) { pn = i; this->arr = p.arr; }
	void swap_process(NewProcess& p) { swap(pn, p.pn); swap(arr, p.arr); }
};
class ProcessQueue {

	//Queue chua cac tien trinh duoc nhap vao
	vector<Process> Queue;

	//NewQueue chua cac tien trinh duoc sap xep theo Arrival time
	vector<NewProcess>NewQueue;

	//ReadyQueue chua cac tien trinh duoc sap xep theo Burst Time
	vector<ReadyProcess>ReadyQueue;

	//Doi vi tri 2 process i va j trong hang doi
	void swap_process(vector<Process> q, int i, int j) {
		Process k = q[i];
		q[i] = q[j];
		q[j] = k;
	}

	//Sort tien trinh trong NewQueue ttheo Arrival Time
	void sort_by_arrival_time() {
		for (int i = 0; i < NewQueue.size() - 1; i++) {
			int min = i;
			for (int j = i + 1; j < NewQueue.size(); j++) {
				if (NewQueue[min].arr > NewQueue[j].arr) min = j;
			}
			if (min != i) NewQueue[min].swap_process(NewQueue[i]);
		}
	}

	//Sort tien trinh trong ReadyQueue theo Burst Time
	void sort_by_burst_time() {
		for (int i = 0; i < ReadyQueue.size() - 1; i++) {
			int min = i;
			for (int j = i + 1; j < ReadyQueue.size(); j++) {
				if (ReadyQueue[min].bur > ReadyQueue[j].bur) min = j;
			}
			if (min != i) ReadyQueue[min].swap_process(ReadyQueue[i]);
		}
	}
public:
	int n = 0;
	int totwt = 0, tottat = 0;

	//Khoi tao process tu dau
	void process_init() {
		Queue.clear();
		cout << "Enter the number of processes:\n";
		cin >> n;
		for (int i = 0; i < n; i++) {
			cout << "Enter the Process Name, Arrival Time, Burst Time:\n";
			int pn, arr, bur; cin >> pn >> arr >> bur;
			Queue.push_back(Process(pn, arr, bur));
			NewQueue.push_back(NewProcess(i, Queue[i]));
		}
	}

	void process_scheduler() {
		sort_by_arrival_time();
		int current_time = NewQueue[0].arr;
		for (int i = 0, remaining = n; remaining;) {

			//Them tat ca cac tien trinh co Arrival Time = current_time vao ReadyQueue
			for (; i < n && NewQueue[i].arr == current_time; i++) {
				ReadyQueue.push_back(ReadyProcess(NewQueue[i].pn, Queue[NewQueue[i].pn]));
			}

			//Neu cac tien trinh khong lien tiep
			if (ReadyQueue.empty()) {
				current_time = NewQueue[i].arr;
			}
			else {
				//Sap Xep lai cac Process trong RaedyQueue
				sort_by_burst_time();
				int session_time = 0;

				if (i < n)
					session_time = min(NewQueue[i].arr - current_time, ReadyQueue[0].bur);
				else
					session_time = ReadyQueue[0].bur;

				/*for (int t = 1; t < ReadyQueue.size(); t++) {
					Queue[ReadyQueue[t].pn].wt += session_time;
				}
				cout << "i: "<<i << " Current time: " << current_time << " Session time: " << session_time << " Current Process: " << Queue[ReadyQueue[0].pn].pn
					<< endl;*/

				if (Queue[ReadyQueue[0].pn].star == -1)
					Queue[ReadyQueue[0].pn].star = current_time;

				ReadyQueue[0].bur -= session_time;
				current_time += session_time;
				if (ReadyQueue[0].bur == 0) {
					Queue[ReadyQueue[0].pn].finish = current_time;
					ReadyQueue.erase(ReadyQueue.begin());
					remaining--;
				}
			}
		}
		for (int i = 0; i < n; i++) {
			Queue[i].tat = Queue[i].finish - Queue[i].arr;
			Queue[i].wt = Queue[i].tat - Queue[i].bur;
		}
	}

	void process_output() {
		printf("PName Arrtime Burtime Start Wait TAT Finish:");
		for (int i = 0; i < n; i++) {
			printf("\n%d%6d\t%6d\t%6d\t%6d\t%6d\t%6d", Queue[i].pn, Queue[i].arr, Queue[i].bur, Queue[i].star, Queue[i].wt, Queue[i].tat, Queue[i].finish);
			totwt += Queue[i].wt;
			tottat += Queue[i].tat;
		}
		cout << "\nAverage Turn Around Time: " << (float)tottat / n << endl;
	}
	ProcessQueue operator=(ProcessQueue pq) {
		this->n = pq.n;
		this->Queue = pq.Queue;
	}
};


int main() {
	ProcessQueue pq;
	pq.process_init();
	pq.process_scheduler();
	pq.process_output();
}
