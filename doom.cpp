///////////////////////// Implement of defs.h //////////////////////
////////////////////////////////////////////////////////////////////

#include "defs.h"

using namespace std;

class DiGraph
{
public:
	class adjVertex
	{
	private:
		int des_vertex, cost;
		adjVertex* next;
		adjVertex* prev;
		friend class DiGraph;

	public:
		adjVertex(int des_vertex, int cost, adjVertex* next, adjVertex* prev)
		{
			this->des_vertex = des_vertex;
			this->cost = cost;
			this->next = next;
			this->prev = prev;
		}
	};

private:
	int max_vertices = 0;
	int current_vertices = 0;
	bool* vertexList = NULL;
	adjVertex** adjTable = NULL;
	bool* visited;

public:
	int sumCost = 0;

	DiGraph(int max_vertices)
	{
		this->max_vertices = max_vertices;
		this->vertexList = new bool[this->max_vertices];
		this->adjTable = new adjVertex * [this->max_vertices];
		this->visited = new bool[this->max_vertices];

		for (int i = 0; i < this->max_vertices; ++i)
		{
			this->vertexList[i] = false;
			this->adjTable[i] = NULL;
			this->visited[i] = false;
		}
	}

	~DiGraph()
	{
		delete[] this->vertexList;
		for (int i = 0; i < this->max_vertices; ++i)
			delete[] this->adjTable[i];
		delete[] this->adjTable;
	}

	//For starting a new traversal
	void resetVisited()
	{
		for (int i = 0; i < this->max_vertices; ++i)
			this->visited[i] = false;
	}

	bool findVertex(int vertex)
	{
		if (vertex < 0 || vertex >= this->max_vertices)
			return false;
		return this->vertexList[vertex];
	}

	//Return -1 if there is no edge between 2 vertices
	int getCost(int src_vertex, int des_vertex)
	{
		if (this->findVertex(src_vertex) == true && this->findVertex(des_vertex) == true)
		{
			adjVertex* current = this->adjTable[src_vertex];
			while (current != NULL)
			{
				if (current->des_vertex == des_vertex)
					return current->cost;
				current = current->next;
			}
		}
		return -1;
	}

	bool isAdjective(int src_vertex, int des_vertex)
	{
		if (this->findVertex(src_vertex) == true &&
			this->findVertex(des_vertex) == true)
		{
			adjVertex* current = this->adjTable[src_vertex];
			while (current != NULL)
			{
				if (current->des_vertex == des_vertex)
					return true;
				current = current->next;
			}
		}
		return false;
	}

	void printGraph()
	{
		cout << "Vertices: ";
		for (int i = 0; i < this->max_vertices; ++i)
		{
			if (this->vertexList[i] == true)
				cout << i << " ";
		}

		cout << "\n";
		cout << "Adjective Table: " << endl;
		cout << "----------------------------" << endl;

		for (int i = 0; i < this->max_vertices; ++i)
		{
			adjVertex* current = this->adjTable[i];
			if (current != NULL)
			{
				cout << i << " | ";
				while (current != NULL)
				{
					cout << "(" << current->des_vertex << "," << current->cost << ") ";
					current = current->next;
				}
				cout << "\n" << "----------------------------" << endl;
			}
		}
	}

	bool insertVertex(int vertex)
	{
		if (this->findVertex(vertex) == false)
		{
			this->vertexList[vertex] = true;
			++this->current_vertices;
			return true;
		}
		return false;
	}

	bool insertEdge(int src_vertex, int des_vertex, int cost)
	{
		if (this->findVertex(src_vertex) == true &&
			this->findVertex(des_vertex) == true)
		{
			if (this->getCost(src_vertex, des_vertex) == -1 && this->getCost(des_vertex, src_vertex) == -1)
				this->sumCost += cost;

			adjVertex* tmp = new adjVertex(des_vertex, cost, this->adjTable[src_vertex], NULL);
			if (this->adjTable[src_vertex] != NULL)
				this->adjTable[src_vertex]->prev = tmp;
			this->adjTable[src_vertex] = tmp;
			return true;
		}
		return false;
	}

	//Delete and return next adjVertex of deleted adjVertex
	adjVertex* deleteEdge(int src_vertex, int des_vertex, int cost)
	{
		adjVertex* current = this->adjTable[src_vertex];
		if (current->des_vertex == this->adjTable[src_vertex]->des_vertex &&
			current->cost == this->adjTable[src_vertex]->cost)
		{
			this->adjTable[src_vertex] = current->next;
			if (this->adjTable[src_vertex] != NULL)
				current->next->prev = NULL;
			current->next = NULL;

			delete current;
			return this->adjTable[src_vertex];
		}
		else
		{
			while (current != NULL)
			{
				if (current->des_vertex == des_vertex && current->cost == cost)
				{
					adjVertex* tmp = current->next;
					current->prev->next = tmp;
					if (tmp != NULL)
						tmp->prev = current->prev;

					current->prev = NULL;
					current->next = NULL;

					delete current;
					return tmp;
				}
			}
		}
		return NULL;
	}

	void deleteEdgesofVertex(int vertex)
	{
		for (int i = 0; i < this->max_vertices; ++i)
		{
			adjVertex* current = this->adjTable[i];
			while (current != NULL)
			{
				if (current->des_vertex == vertex || i == vertex)
					current = this->deleteEdge(i, current->des_vertex, current->cost);
				else
					current = current->next;
			}
		}
	}

	//Return src_vertex if it has no adjacent vertex or has only cost to adjacent vertex = 99
	int getlowestCostVertex_skip99(int src_vertex)
	{
		if (this->findVertex(src_vertex) == true)
		{
			adjVertex* current = this->adjTable[src_vertex];
			if (current != NULL)
			{
				int minCost = current->cost;
				int des_vertex = current->des_vertex;
				while (current != NULL)
				{
					if ((current->cost < minCost && current->cost != 99 && this->visited[current->des_vertex] == false) || minCost == 99)
					{
						minCost = current->cost;
						des_vertex = current->des_vertex;
					}
					current = current->next;
				}
				if (minCost == 99) return src_vertex;
				return des_vertex;
			}
			return src_vertex;
		}
		return src_vertex;
	}

	//Return src_vertex if it has no adjacent vertex or has only cost to adjacent vertex = 99
	int gethighestCostVertex_skip99(int src_vertex, int exclude_src_vertex, int exclude_des_vertex, int exclude_cost, bool exclude)
	{
		if (this->findVertex(src_vertex) == true)
		{
			adjVertex* current = this->adjTable[src_vertex];
			if (current != NULL)
			{
				int maxCost = current->cost;
				int des_vertex = current->des_vertex;
				while (current != NULL)
				{
					if ((current->cost > maxCost && current->cost != 99 && this->visited[current->des_vertex] == false) || 
						maxCost == 99 ||
						//Avoid directed path
						(src_vertex == exclude_src_vertex && des_vertex == exclude_des_vertex && maxCost == exclude_cost && exclude == true))					
					{
						maxCost = current->cost;
						des_vertex = current->des_vertex;
					}
					current = current->next;
				}
				if (maxCost == 99) return src_vertex;
				return des_vertex;
			}
			return src_vertex;
		}
		return src_vertex;
	}

	//Find shortest or farest path. If there is no path return -1
	int findPath(int src_vertex, int des_vertex, bool min_max, bool exclude)
	{
		if (this->findVertex(src_vertex) == true && this->findVertex(des_vertex) == true)
		{
			if (src_vertex == des_vertex)
				return 0;

			int current = src_vertex;
			int sumCost = 0;
			this->visited[current] = true;

			while (current != des_vertex)
			{
				int nextVertex;
				if (min_max == 0)
					nextVertex = this->getlowestCostVertex_skip99(current);
				else
					nextVertex = this->gethighestCostVertex_skip99(current, src_vertex, des_vertex, this->getCost(src_vertex, des_vertex), exclude);

				if (nextVertex == current || this->visited[nextVertex] == true)
					return -1;
				this->visited[nextVertex] = true;
				

				sumCost += this->getCost(current, nextVertex);
				current = nextVertex;
			}

			int directedPath = getCost(src_vertex, des_vertex);
			if (min_max == false)
			{
				if (directedPath != -1 && directedPath != 99 && directedPath < sumCost)
					return directedPath;
			}
			else
			{
				if (directedPath != -1 && directedPath != 99 && directedPath > sumCost)
					return directedPath;
			}
			return sumCost;
		}
		return -1;
	}

	//Return src_vertex if it has no adjacent vertex or vertex is visited
	int getlowestCostVertex(int src_vertex)
	{
		if (this->findVertex(src_vertex) == true)
		{
			adjVertex* current = this->adjTable[src_vertex];
			if (current != NULL)
			{
				int minCost;
				int des_vertex;;

				//Find first not visited vertex
				while (current != NULL)
				{
					if (this->visited[current->des_vertex] == false)
					{
						minCost = current->cost;
						des_vertex = current->des_vertex;
						break;
					}
					current = current->next;
				}

				//If there is no not visited vertex
				if (current == NULL) return src_vertex;

				while (current != NULL)
				{
					if (current->cost < minCost && this->visited[current->des_vertex] == false)
					{
						minCost = current->cost;
						des_vertex = current->des_vertex;
					}
					current = current->next;
				}
				return des_vertex;
			}
			return src_vertex;
		}
		return src_vertex;
	}

	//Return minimum cost to a not visited vertex, return -1 if all vertices are visited
	int getMinimumCost()
	{
		int minCost = 999999999;
		for (int i = 0; i < this->max_vertices; ++i)
		{
			if (this->visited[i] == true)
			{
				int tmp_vertex = this->getlowestCostVertex(i);
				//No adjective vertex or vertex is visited
				if (tmp_vertex == i || visited[tmp_vertex] == true)
					continue;
				//Vertex is not visited
				int tmp_cost = this->getCost(i, tmp_vertex);
				if (tmp_cost < minCost)
					minCost = tmp_cost;
			}
		}
		if (minCost != 999999999) return minCost;
		return -1;
	}

	//Return a not visited vertex which cost is minimum, return -1 if all vertices are visited
	int getMinimumCostVertex()
	{
		int des_vertex = -1;
		int minCost = 999999999;
		for (int i = 0; i < this->max_vertices; ++i)
		{
			if (this->visited[i] == true)
			{
				int tmp_vertex = this->getlowestCostVertex(i);
				//No adjective vertex or vertex is visited
				if (tmp_vertex == i || visited[tmp_vertex] == true)
					continue;
				//Vertex is not visited
				int tmp_cost = this->getCost(i, tmp_vertex);
				if (tmp_cost < minCost)
				{
					minCost = tmp_cost;
					des_vertex = tmp_vertex;
				}
			}
		}
		return des_vertex;
	}

	//Return -1 if no spanning tree
	int minimumSpanningTree()
	{
		for (int i = 0; i < this->max_vertices; ++i)
		{
			if (this->findVertex(i) == true)
			{
				this->visited[i] = true;
				break;
			}
		}

		int countEdge = 0;
		int sumCost = 0;
		while (countEdge != this->current_vertices - 1)
		{
			int cost = this->getMinimumCost();
			int visiting_vertex = this->getMinimumCostVertex();
			if (cost != -1 && visiting_vertex != -1)
			{
				sumCost += cost;
				this->visited[visiting_vertex] = true;
				++countEdge;
			}
			else
				break;
		}
		if (countEdge != this->current_vertices - 1)
			return -1;
		return sumCost;
	}
};

bool Volcano(int N)
{
	if (N < 0)
		return true;
	return false;
}

string defineCharacter(int HP)
{
	if (HP == 888)
		return "Legolas";
	else if (HP == 999)
		return "Gandalf";
	else if (HP == 777)
		return "Aragorn";
	else if (HP > 0 && HP < 200)
		return "AmateurKnight";
	else if (HP >= 200 && HP < 600)
		return "Ringbearer";
	else if (HP >= 600 && HP < 999)
		return "Ranger";
	return "";
}

int getHP(int N)
{
	return abs(N);
}

int getStartPole(int eventcode)
{
	return eventcode / 100000;
}

int getEndPole(int eventcode)
{
	return eventcode % 100;
}

int getCost(int eventcode)
{
	return (eventcode / 100) % 1000;
}

int getDarkPortal(eventList* pEventList)
{
	if (pEventList == NULL)
		return -1;

	return getStartPole(pEventList->nEventCode);
}

int getMountDoom(eventList* pEventList)
{
	if (pEventList == NULL)
		return -1;

	while (pEventList->pNext != NULL)
		pEventList = pEventList->pNext;
	return getEndPole(pEventList->nEventCode);
}

int getGaladrielStartPole(eventList* pEventList)
{
	if (pEventList == NULL)
		return -1;

	while (pEventList != NULL)
	{
		if (getCost(pEventList->nEventCode) == 0)
			return getStartPole(pEventList->nEventCode);
		pEventList = pEventList->pNext;
	}
	return -1;
}

int getGaladrielEndPole(eventList* pEventList)
{
	if (pEventList == NULL)
		return -1;

	while (pEventList != NULL)
	{
		if (getCost(pEventList->nEventCode) == 0)
			return getEndPole(pEventList->nEventCode);
		pEventList = pEventList->pNext;
	}
	return -1;
}

int getSarumanStartPole(eventList* pEventList)
{
	if (pEventList == NULL)
		return -1;

	while (pEventList != NULL)
	{
		if (getCost(pEventList->nEventCode) == 99)
			return getStartPole(pEventList->nEventCode);
		pEventList = pEventList->pNext;
	}
	return -1;
}

int getSarumanEndPole(eventList* pEventList)
{
	if (pEventList == NULL)
		return -1;

	while (pEventList != NULL)
	{
		if (getCost(pEventList->nEventCode) == 99)
			return getEndPole(pEventList->nEventCode);
		pEventList = pEventList->pNext;
	}
	return -1;
}

DiGraph* generateMordor(eventList* pEventList, bool volcano)
{
	DiGraph* Mordor = new DiGraph(100);

	while (pEventList != NULL)
	{
		int startPole = getStartPole(pEventList->nEventCode);
		int endPole = getEndPole(pEventList->nEventCode);
		int cost = getCost(pEventList->nEventCode);

		Mordor->insertVertex(startPole);
		Mordor->insertVertex(endPole);
		Mordor->insertEdge(startPole, endPole, cost);
		if (volcano == false)
			Mordor->insertEdge(endPole, startPole, cost);

		pEventList = pEventList->pNext;
	}
	return Mordor;
}

int countEagleServices(eventList* pEventList)
{
	int result = 0;
	while (pEventList != NULL)
	{
		++result;
		pEventList = pEventList->pNext;
	}
	return result;
}

int AmateurKnight(int HP, eventList* pEventList, DiGraph* Mordor)
{
	int eagleServices = countEagleServices(pEventList);

	return HP + eagleServices + Mordor->sumCost;
}

int RingbearerTravel(int HP, DiGraph* Mordor, int DarkPortal, int MountDoom, int Galadriel_StartPole, int Galadriel_Endpole)
{
	if (Galadriel_StartPole == -1 && Galadriel_Endpole == -1)
	{
		int cost = Mordor->findPath(DarkPortal, MountDoom, false, true);
		if (HP >= cost) return cost;
		return -1;
	}

	int first_cost = Mordor->findPath(DarkPortal, Galadriel_StartPole, false, true);
	int second_cost = Mordor->findPath(Galadriel_Endpole, MountDoom, false, true);

	if (first_cost == -1 || second_cost == -1 || first_cost + second_cost >= HP)
		return -1;
	return first_cost + second_cost;
}

int RangerTravel(int HP, DiGraph* Mordor, int DarkPortal, int MountDoom, int Galadriel_StartPole, int Galadriel_EndPole)
{
	if (Galadriel_StartPole == -1 && Galadriel_EndPole == -1)
	{
		int cost = Mordor->findPath(DarkPortal, MountDoom, true, true);
		if (HP >= cost) return cost;
	}

	int first_cost = Mordor->findPath(DarkPortal, Galadriel_StartPole, true, true);
	int second_cost = Mordor->findPath(Galadriel_EndPole, MountDoom, true, false);

	if (first_cost == -1 || second_cost == -1 || first_cost + second_cost > HP)
		return -1;
	return first_cost + second_cost;
}

int LegolasTravel(int HP, DiGraph* Mordor, int DarkPortal, int MountDoom, int Galadriel_StartPole, int Galadriel_Endpole, int Saruman_StartPole, int Saruman_EndPole)
{
	if ((Galadriel_StartPole != -1 && Saruman_StartPole == -1) ||
		(Galadriel_StartPole == -1 && Saruman_StartPole != -1))
		return RangerTravel(HP, Mordor, DarkPortal, MountDoom, Galadriel_StartPole, Galadriel_Endpole);

	else if (Galadriel_StartPole != 1 && Saruman_StartPole != -1)
	{
		//Saruman--->Galadriel
		int P1_first_cost = Mordor->findPath(DarkPortal, Saruman_StartPole, true, true);
		int P1_second_cost = Mordor->findPath(Saruman_EndPole, Galadriel_StartPole, true, false);
		int P1_third_cost = Mordor->findPath(Galadriel_Endpole, MountDoom, true, false);
		if (DarkPortal == Saruman_StartPole)
			P1_first_cost = 0;
		int P1_cost = P1_first_cost + P1_second_cost + P1_third_cost;

		if (P1_first_cost == -1 ||
			P1_second_cost == -1 ||
			P1_third_cost == -1 ||
			P1_cost > HP)
			P1_cost = -1;

		//Galadriel--->Saruman
		Mordor->resetVisited();
		int P2_first_cost = Mordor->findPath(DarkPortal, Galadriel_StartPole, true, true);
		int P2_second_cost = Mordor->findPath(Galadriel_Endpole, Saruman_StartPole, true, false);
		int P2_third_cost = Mordor->findPath(Saruman_EndPole, MountDoom, true, false);
		if (DarkPortal == Galadriel_StartPole)
			P2_first_cost = 0;
		int P2_cost = P2_first_cost + P2_second_cost + P2_third_cost;

		if (P2_first_cost == -1 ||
			P2_second_cost == -1 ||
			P2_third_cost == -1 ||
			P2_cost > HP)
			P2_cost = -1;
		return max(P1_cost, P2_cost) + 99;
	}
	return -1;
}

int GandalfTravel(DiGraph* Mordor)
{
	return Mordor->minimumSpanningTree();
}

int travel (int N, eventList* pEventList)
{
	//Get HP, character and create Mordor
	int HP = getHP(N);
	string character = defineCharacter(HP);
	DiGraph* Mordor = generateMordor(pEventList, Volcano(N));

	//Mordor->printGraph();

	//Get DarkPortal and MountDoom
	int DarkPortal = getDarkPortal(pEventList);
	int MountDoom = getMountDoom(pEventList);
	
	//For Galadriel
	int Galadriel_StartPole = getGaladrielStartPole(pEventList);
	int Galadriel_EndPole = getGaladrielEndPole(pEventList);

	//For Saruman
	int Saruman_StartPole = getSarumanStartPole(pEventList);
	int Saruman_Endpole = getSarumanEndPole(pEventList);

	if (character == "AmateurKnight")
		return AmateurKnight(HP, pEventList, Mordor);
	else if (character == "Ringbearer")
		return RingbearerTravel(HP, Mordor, DarkPortal, MountDoom, Galadriel_StartPole, Galadriel_EndPole);
	else if (character == "Ranger")
		return RangerTravel(HP, Mordor, DarkPortal, MountDoom, Galadriel_StartPole, Galadriel_EndPole);
	else if (character == "Legolas")
		return LegolasTravel(HP, Mordor, DarkPortal, MountDoom, Galadriel_StartPole, Galadriel_EndPole, Saruman_StartPole, Saruman_Endpole);
	else if (character == "Gandalf")
		return GandalfTravel(Mordor);

	return 0;
}

bool checkSCC (int N, eventList* pEventList)
{
	return 0;
}
