#include "CENGFlight.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void CENGFlight::PrintCanNotHalt(const std::string& airportFrom,
                                 const std::string& airportTo,
                                 const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void CENGFlight::PrintCanNotResumeFlight(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void CENGFlight::PrintFlightFoundInCache(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  " is found in cache." << std::endl;
}

void CENGFlight::PrintFlightCalculated(const std::string& airportFrom,
                                       const std::string& airportTo,
                                       bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  "." << std::endl;
}

void CENGFlight::PrintPathDontExist(const std::string& airportFrom,
                                    const std::string& airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo <<"\"." << std::endl;
}

void CENGFlight::PrintSisterAirlinesDontCover(const std::string& airportFrom)
{
    std::cout << "Could not able to generate sister airline list from \""
              << airportFrom <<"\"." << std::endl;
}

void CENGFlight::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void CENGFlight::PrintCache()
{
    lruTable.PrintTable();
}

CENGFlight::CENGFlight(const std::string& flightMapPath)
    : navigationMap(flightMapPath)
{}

//=======================//
//          TODO         //
//=======================//
void CENGFlight::HaltFlight(const std::string& airportFrom,
                            const std::string& airportTo,
                            const std::string& airlineName)
{
    if(navigationMap.edgeExists(airportFrom,airportTo,airlineName)){
        GraphEdge temp2=navigationMap.getEdge(airportFrom,airportTo,airlineName);
    navigationMap.RemoveEdge(airlineName,airportFrom,airportTo);
    HaltedFlight temp;
    temp.airportFrom=airportFrom;
    temp.airportTo=airportTo;
    temp.airline=airlineName;
    temp.w0=temp2.weight[0];
    temp.w1=temp2.weight[1];
    haltedFlights.push_back(temp);
    }
    else{
        PrintCanNotHalt(airportFrom,airportTo,airlineName);
    }
}

    // (Direct Function call)
void CENGFlight::ContinueFlight(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName)
{
    if(navigationMap.vertexExists(airportFrom,airportTo)&&!navigationMap.edgeExists(airportFrom,airportTo,airlineName)){
        for(int i=0;i<haltedFlights.size();i++){
            if(haltedFlights[i].airportFrom==airportFrom&&haltedFlights[i].airportTo==airportTo&&haltedFlights[i].airline==airlineName){
                navigationMap.AddEdge(airlineName,airportFrom,airportTo,haltedFlights[i].w0,haltedFlights[i].w1);
                return;
            }
        }
    }
    PrintCanNotResumeFlight(airportFrom,airportTo,airlineName);
    
}

void CENGFlight::FindFlight(const std::string& startAirportName,
                            const std::string& endAirportName,
                            float alpha)
{
    std::vector<int> myVec;
    if(navigationMap.vertexExists(startAirportName,endAirportName)){
        int s=navigationMap.getIndex(startAirportName);
        int t=navigationMap.getIndex(endAirportName);
        if(alpha==0){
            if(lruTable.Find(myVec,s,t,true,true)){
                PrintFlightFoundInCache(startAirportName,endAirportName,true);
                navigationMap.PrintPath(myVec,0,true);
            }
            else{
                if(navigationMap.HeuristicShortestPath(myVec,startAirportName,endAirportName,0)){
                    lruTable.Insert(myVec,true);
                    PrintFlightCalculated(startAirportName,endAirportName,true);
                    navigationMap.PrintPath(myVec,0,true);
                }
                else{
                    PrintPathDontExist(startAirportName,endAirportName);
                }
            }
        }
        else if(alpha==1){
            if(lruTable.Find(myVec,s,t,false,true)){
                PrintFlightFoundInCache(startAirportName,endAirportName,false);
                navigationMap.PrintPath(myVec,1,true);
            }
            else{
                if(navigationMap.HeuristicShortestPath(myVec,startAirportName,endAirportName,1)){
                    lruTable.Insert(myVec,false);
                    PrintFlightCalculated(startAirportName,endAirportName,false);
                    navigationMap.PrintPath(myVec,1,true);
                }
                else{
                    PrintPathDontExist(startAirportName,endAirportName);
                }
            }
        }
        else{
            if(navigationMap.HeuristicShortestPath(myVec,startAirportName,endAirportName,alpha)){
                    navigationMap.PrintPath(myVec,alpha,true);
                }
            else{
                PrintPathDontExist(startAirportName,endAirportName);
            }
        }
    }
}

void CENGFlight::FindSpecificFlight(const std::string& startAirportName,
                                    const std::string& endAirportName,
                                    float alpha,
                                    const std::vector<std::string>& unwantedAirlineNames) const
{
    std::vector<int> myVec;
    if(navigationMap.vertexExists(startAirportName,endAirportName)){
        int s=navigationMap.getIndex(startAirportName);
        int t=navigationMap.getIndex(endAirportName);
        if(alpha==0){
            if(navigationMap.FilteredShortestPath(myVec,startAirportName,endAirportName,0,unwantedAirlineNames)){
                    navigationMap.PrintPath(myVec,0,true);
            }
            else{
                PrintPathDontExist(startAirportName,endAirportName);
            }
        }
        else if(alpha==1){
            if(navigationMap.FilteredShortestPath(myVec,startAirportName,endAirportName,1,unwantedAirlineNames)){
                    navigationMap.PrintPath(myVec,1,true);
            }
            else{
                PrintPathDontExist(startAirportName,endAirportName);
            }
        }
        else{
            if(navigationMap.FilteredShortestPath(myVec,startAirportName,endAirportName,alpha,unwantedAirlineNames)){
                    navigationMap.PrintPath(myVec,alpha,true);
                }
            else{
                PrintPathDontExist(startAirportName,endAirportName);
            }
        }
    }
}

void CENGFlight::FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const
{
    if(navigationMap.vertex1Exists(airportName)){
        std::vector<GraphVertex> list=navigationMap.getList();
        int s=navigationMap.getIndex(airportName);
        std::vector<int> visited;//indexes of visited vertexes
        bool a=false;
        for(int i=0;i<list[s].edges.size();i++){
            if(list[s].edges[i].name==startAirlineName){
                a=true;
            }
        }
        if(a){
            std::vector<GraphVertex> contains;
            std::vector<GraphVertex> containsVisited;
            GraphVertex dummy;
            contains.push_back(list[s]);
            while(contains.size()!=0){
                dummy=contains[0];
                contains.erase(contains.begin());
                bool c=true;
                for(int j=0;j<containsVisited.size();j++){
                    if(containsVisited[j].name==dummy.name){
                        c=false;
                    }
                }
                if(c){
                    for(int i=0;i<dummy.edges.size();i++){
                        if(dummy.edges[i].name==startAirlineName){
                            contains.push_back(list[dummy.edges[i].endVertexIndex]);
                        }
                    }
                    bool o=true;
                    for(int i=0;i<visited.size();i++){
                        if(visited[i]==navigationMap.getIndex(dummy.name)){
                            o=false;
                        }
                    }
                    if(o){
                        visited.push_back(navigationMap.getIndex(dummy.name));
                    }
                    containsVisited.push_back(dummy);
                }
            }
            int k=0;
            int max;
            int l;
            while(visited.size()!=list.size()){
                if(k==visited.size()){
                    airlineNames.clear();
                    PrintSisterAirlinesDontCover(airportName);
                }
                k=visited.size();
                max=0;
                int maxIndex;
                int maxEdgeIndex;
                for(int x=0;x<visited.size();x++){//getting the vertex with max non visited neighbour
                    l=0;
                    for(int y=0;y<list[visited[x]].edges.size();y++){
                        bool g=true;
                        for(int m=0;m<visited.size();m++){
                            if(visited[m]==list[visited[x]].edges[y].endVertexIndex){
                                g=false;
                            }
                        }
                        if(g){
                            l++;
                        }
                    }
                    if(max<l){
                        max=l;
                        maxIndex=visited[x];
                    }
                }
                for(int i=0;i<list[maxIndex].edges.size();i++){//getting the edge index;
                    bool g=true;
                    for(int m=0;m<visited.size();m++){
                        if(visited[m]==list[maxIndex].edges[i].endVertexIndex){
                            g=false;
                        }
                    }
                    if(g){
                        maxEdgeIndex=i;
                        break;
                    }
                }
                contains.push_back(list[maxIndex]);
                containsVisited.clear();
                while(contains.size()!=0){
                    dummy=contains[0];
                    contains.erase(contains.begin());
                    bool c=true;
                    for(int j=0;j<containsVisited.size();j++){
                        if(containsVisited[j].name==dummy.name){
                            c=false;
                        }
                    }
                    if(c){
                        for(int i=0;i<dummy.edges.size();i++){
                            if(dummy.edges[i].name==dummy.edges[maxEdgeIndex].name){
                                contains.push_back(list[dummy.edges[i].endVertexIndex]);
                            }
                        }
                        bool o=true;
                        for(int i=0;i<visited.size();i++){
                            if(visited[i]==navigationMap.getIndex(dummy.name)){
                                o=false;
                            }
                        }
                        if(o){
                            visited.push_back(navigationMap.getIndex(dummy.name));
                        }
                        containsVisited.push_back(dummy);
                    }
                }
                airlineNames.push_back(dummy.edges[maxEdgeIndex].name);
            }
            for(int i=0;i<airlineNames.size();i++){
                if(airlineNames[i]==startAirlineName){
                    airlineNames.erase(airlineNames.begin()+i);
                }
            }
        }
        else{
            PrintSisterAirlinesDontCover(airportName);
        }
    }
    else{
        PrintSisterAirlinesDontCover(airportName);
    }
}

int CENGFlight::FurthestTransferViaAirline(const std::string& airportName,
                                           const std::string& airlineName) const
{
    if(navigationMap.vertex1Exists(airportName)){
        return navigationMap.MaxDepthViaEdgeName(airportName,airlineName);
    }
    else{
        return -1;
    }
}