#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sys/stat.h>
using namespace std;

string chainID;
string networkID;
string nonce;
string difficulty;
string gas;
string gethLoc;
string dataLoc;

string *parameter;
int totalParametersCount;

void ui_Interactive();
void ui_Unattended();
void ui_WriteConfig();
void ui_Remove();
void ui_Create();
bool check_chainID();
bool check_networkID();
bool check_Nonce();
bool check_Difficulty();
bool check_GasLimit();
bool check_ChainExist();
void config_Load();
void config_Reset();
void auto_networkID();
void auto_Nonce();
void auto_Difficulty();
void auto_GasLimit();

int main(int argc, char **argv)
{
    config_Load();
    if (argc < 2)
	{
        cout << endl;
		cout << "\tEasy Create Chain Tool v3 for Ethereum 1.9 / Linux" << endl;
		cout << "\tUsage : createChain [-i/-c/-r] [chain ID] [Options]" << endl;
		cout << "\t\t-i Create a chain interactively." << endl;
		cout << "\t\t-c [chain ID] Create a chain, require Options." << endl;
		cout << "\t\t-r [chain ID] Remove a chain." << endl;
		cout << "\t[Options] :" << endl;
		cout << "\t\t-w [Network ID] Determine the network ID for the chain." << endl;
		cout << "\t\t-n [Nounce] Determine the nonce value for the chain." << endl;
		cout << "\t\t-d [Difficulty] Determine the difficulty value for the chain." << endl;
		cout << "\t\t-g [GasLimit] Determine the GasLimit for the chain." << endl;
        cout << endl;
		return 0;
	}

    parameter = new string[argc -1];
    for (int i = 1; i < argc; i++)
    {
        string subparameter(argv[i]);
        parameter[i-1].assign(subparameter);
        //parameter[i-1] = new string(argv[i]);
    }
    totalParametersCount = argc -1;

    if (parameter[0].compare("-i") == 0)
    {
        cout << "Interactive Mode" << endl;
        ui_Interactive();
    }
    else if (parameter[0].compare("-c") == 0)
    {
        if (argc < 3) 
        {
            cout << "Too few arguments !" << endl;
            exit(1);
        }
        cout << "Unattended Mode" << endl;
        ui_Unattended();
    }
    else if (parameter[0].compare("-r") == 0)
    {
        if (argc < 3) 
        {
            cout << "Too few arguments !" << endl;
            exit(1);
        }
        ui_Remove();
    }
}

void ui_Interactive()
{
    do
    {
        cout << "Enter preferred chain ID" << endl;
        getline(cin, chainID);
    } while (!check_chainID());
    do
    {
        cout << "Enter preferred network ID. X to auto setup" << endl;
        getline(cin, networkID);
        if (networkID.compare("X") == 0) 
        {
            auto_networkID();
        }
    } while (!check_networkID());
    do
    {
        cout << "Enter preferred GasLimit. X to auto setup" << endl;
        getline(cin, nonce);
        if (nonce.compare("X") == 0)
        {
            auto_Nonce();
        }
    } while (!check_Nonce());
    do
    {
        cout << "Enter preferred GasLimit. X to auto setup" << endl;
        getline(cin, gas);
        if (gas.compare("X") == 0)
        {
            auto_GasLimit();
        }
    } while (!check_GasLimit());
    do
    {
        cout << "Enter preffered Difficulty. X to auto setup" << endl;
        getline(cin, difficulty);
        if (difficulty.compare("X") == 0)
        {
            auto_Difficulty();
        }
    } while (!check_Difficulty());
    ui_WriteConfig();
}

void ui_Unattended()
{
    chainID.assign(parameter[1]);
    if (!check_chainID())
    {
        cout << "You specified a bad chainID" << endl;
        exit(1);
    }

    bool networkIDAuto = true;
    bool difficultyAuto = true;
    bool nonceAuto = true;
    bool gasAuto = true;

    if ( totalParametersCount > 2)
    {
        string *options = new string[totalParametersCount  - 2];
        for (int i = 2; i < totalParametersCount ; i++)
        {
            (options[i - 2]).assign(parameter[i]);
        }

        int optionsSize = totalParametersCount -2;

        int remainder = optionsSize % 2;
        if (remainder != 0)
        {
            cout << "Bad parameters" << endl;
            exit(1);
        }
        for (int i = 0; i < optionsSize; i++)
        {
            if (options[i].compare("-w") == 0)
            {
                i++;
                if (i == optionsSize)
                {
                    cout << "Bad Parameters" << endl;
                    exit(1);
                }
                
                networkID.assign(options[i]);
                if (!check_networkID())
                {
                    cout << "Bad networkID" << endl;
                }
                else
                {
                    networkIDAuto = false;
                }
            }
            else if (options[i].compare("-n") == 0)
            {
                i++;
                if (i == optionsSize)
                {
                    cout << "Bad Parameters" << endl;
                    exit(1);
                }

                nonce.assign(options[i]);
                if (!check_Nonce())
                {
                    cout << "Bad Nonce" << endl;
                    exit(1);
                }
                else
                {
                    nonceAuto = false;
                }
            }

            else if (options[i].compare("-d") == 0)
            {
                i++;
                if (i == optionsSize)
                {
                    cout << "Bad Parameters" << endl;
                    exit(1);
                }

                difficulty.assign(options[i]);
                if (!check_Difficulty())
                {
                    cout << "Bad Difficulty" << endl;
                }
                else
                {
                    difficultyAuto = false;
                }

            }

            else if (options[i].compare("-g") == 0)
            {
                i++;
                if (i == optionsSize)
                {
                    cout << "Bad Parameters" << endl;
                    exit(1);
                }

                gas.assign(options[i]);
                if (!check_GasLimit())
                {
                    cout << "Bad GasLimit" << endl;
                }
                else
                {
                    gasAuto = false;
                }
            }
        }

    }
    if (networkIDAuto)
    {
        auto_networkID();
    }
    if (difficultyAuto)
    {
        auto_Difficulty();
    }
    if (nonceAuto)
    {
        auto_Nonce();
    }
    if (gasAuto)
    {
        auto_GasLimit();
    }
    ui_WriteConfig();
}

void ui_Create()
{
    if (check_ChainExist()) 
    {
        cout << "Chain Exists !" << endl;
        exit(1);
    }
    else
    {
        string command = "mkdir ";
        command.append(dataLoc);
        if (dataLoc.at(dataLoc.length() -1) != '/')
        {
            command.append("/");
        }
        command.append("chain-");
        command.append(chainID);
        system(command.c_str());
    }
}


void ui_WriteConfig()
{
    ui_Create();
    ofstream genesisFile;
    string gFFilePath = "";
    gFFilePath.append(dataLoc);
    if (dataLoc.at(dataLoc.length() -1) != '/')
    {
        gFFilePath.append("/");
    }
    gFFilePath.append("chain-");
    gFFilePath.append(chainID);
    gFFilePath.append("/genesis.json");

    genesisFile.open(gFFilePath);
    genesisFile << "{" << "\n";
	genesisFile << "\t" << "\"config\": {" << "\n";
	genesisFile << "\t\t" << "\"chainId\": " << chainID.c_str() << "," << "\n";
	genesisFile << "\t\t" << "\"homesteadBlock\": 0," << "\n";
	genesisFile << "\t\t" << "\"eip150Block\": 0," << "\n";
	genesisFile << "\t\t" << "\"eip155Block\": 0," << "\n";
	genesisFile << "\t\t" << "\"eip158Block\": 0" << "\n";
	genesisFile << "\t" << "}," << "\n";
	genesisFile << "\t" << "\"nonce\": \"0x" << nonce.c_str() << "\"," << "\n";
	genesisFile << "\t" << "\"timestamp\": \"0x0\"," << "\n";
	genesisFile << "\t" << "\"extraData\": \"0x\"," << "\n";
	genesisFile << "\t" << "\"difficulty\": \"0x" << difficulty.c_str() << "\"," << "\n";
	genesisFile << "\t" << "\"gasLimit\": \"0x" << gas.c_str() << "\"," << "\n";
	genesisFile << "\t" << "\"mixhash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\"," << "\n";
	genesisFile << "\t" << "\"parentHash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\"," << "\n";
	genesisFile << "\t" << "\"alloc\": {}" << "\n";
	genesisFile << "}";
	genesisFile.flush();
	genesisFile.close();

    ofstream bootScript;
    string bSfilePath = "";
    bSfilePath.append(dataLoc);
    if (dataLoc.at(dataLoc.length() -1) != '/')
    {
        bSfilePath.append("/");
    }
    bSfilePath.append("chain-");
    bSfilePath.append(chainID);
    bSfilePath.append("/boot.sh");
    bootScript.open(bSfilePath);
    bootScript << gethLoc.c_str() << " console --datadir " << dataLoc.c_str();
    if (dataLoc.at(dataLoc.length() -1) != '/')
    {
        bootScript << '/';
    }
    bootScript << "chain-" << chainID.c_str() << "/ --networkid " << networkID.c_str() << " --port 30303 --nodiscover" << "\n";
    bootScript.flush();
    bootScript.close();

	string bScmd = "chmod 775 ";
	bScmd.append(bSfilePath);
	system(bScmd.c_str());

    ofstream initScript;
    string iSfilePath = "";
    iSfilePath.append(dataLoc);
    if (dataLoc.at(dataLoc.length() -1) != '/')
    {
        iSfilePath.append("/");
    }
    iSfilePath.append("chain-");
    iSfilePath.append(chainID);
    iSfilePath.append("/init.sh");
    initScript.open(iSfilePath);
    initScript << gethLoc.c_str() << " --datadir " << dataLoc.c_str();
    if (dataLoc.at(dataLoc.length() -1) != '/')
    {
        initScript << '/';
    }
    initScript << "chain-" << chainID.c_str() << "/ init ";
    initScript << dataLoc.c_str();
    if (dataLoc.at(dataLoc.length() -1) != '/')
    {
        initScript << '/';
    }
    initScript << "chain-" << chainID.c_str() << "/genesis.json" << "\n";
    initScript.flush();
    initScript.close();

	string iScmd = "chmod 775 ";
	iScmd.append(iSfilePath);
	system(iScmd.c_str());

    cout << "Use " << iSfilePath << " to initailize your private chain." << endl;
    cout << "After init, use " << bSfilePath << " to boot your private chain." << endl;
}

void ui_Remove()
{
    chainID.assign(parameter[1]);
    if (!check_chainID())
    {
        cout << "You specified a bad chainID" << endl;
        exit(1);
    }
    if (check_ChainExist())
    {

        string command = "rm -rf ";
        command.append(dataLoc);
        if (dataLoc.at(dataLoc.length() -1) != '/')
        {
            command.append("/");
        }
        command.append("chain-");
        command.append(chainID);
        system(command.c_str());
    }
    else
    {
        cout << "You specify a non-existed Chain to remove." << endl;
    }
}

bool check_chainID()
{
    if (chainID.length() > 5)
	{
		cout << "Unsupported Chain ID" << endl;
		return false;
	}
	else if (chainID.length() < 2)
	{
		cout << "Do not assign a Chain ID smaller than 2 digits" << endl;
		return false;
	}
	else if (chainID.at(0) == '0')
	{
		cout << "You can not specify a Chain ID start with 0" << endl;
		return false;
	}
	return chainID.find_first_not_of("0123456789") == std::string::npos;
}

bool check_networkID()
{
    if (networkID.length() > 5)
	{
		cout << "Unsupported Network ID" << endl;
		return false;
	}
	else if (networkID.length() < 2)
	{
		cout << "Do not assign a Network ID smaller than 2 digits" << endl;
		return false;
	}
	else if (chainID.at(0) == '0')
	{
		cout << "You can not specify a Network ID start with 0" << endl;
		return false;
	}
	return networkID.find_first_not_of("0123456789") == std::string::npos;
}

bool check_Nonce()
{
    return gas.find_first_not_of("0123456789abcdef") == std::string::npos;
}

bool check_Difficulty()
{
    if (difficulty.at(0) == '0')
	{
		cout << "You can not set a difficulty starts with 0" << endl;
		return false;
	}
	if (difficulty.length() > 6)
	{
		cout << "You specified a difficulty value too high, which may result a low performance system";
	}
	return difficulty.find_first_not_of("0123456789") == std::string::npos;
}

bool check_GasLimit()
{
    if (gas.at(0) == '0')
	{
		cout << "You can not set a GasLimit starts with 0" << endl;
		return false;
	}
	if (gas.length() > 8)
	{
		cout << "Unsupported GasLimit" << endl;
		return false;
	}
	return gas.find_first_not_of("0123456789abcdef") == std::string::npos;
}

bool check_ChainExist()
{
    struct stat buffer;
    string folder = "";
    folder.append(dataLoc);
    folder.append("chain-");
    folder.append(chainID);
    return (stat (folder.c_str(), &buffer) == 0);
}

void config_Load()
{
    ifstream ifc;
	ifc.open("config.ini");
	if (ifc.good())
	{
		while (ifc.peek() != EOF)
		{
			string temp;
			getline(ifc, temp);
			if (temp.find("geth=") != std::string::npos)
			{
				gethLoc.assign(temp.substr(5, temp.length()));
			}
			else if (temp.find("data=") != std::string::npos)
			{
				dataLoc.assign(temp.substr(5, temp.length()));
			}
		}
		if (gethLoc.length() == 0 || dataLoc.length() == 0)
		{
			cout << "Bad config file !";
			config_Reset();
		}
		ifc.close();
	}
	else
	{
		ifc.close();
		cout << "No config.ini file!";
		config_Reset();
	}
}

void config_Reset()
{
    cout << "The location of geth executable ?" << endl;
	string toGethLoc;
	getline(cin, toGethLoc);
	gethLoc.assign(toGethLoc);
	cout << "The location to store chain data ?" << endl;
	string toDataLoc;
	getline(cin, toDataLoc);
	dataLoc.assign(toDataLoc);
	cout << "Config Reset !" << endl;
	ofstream ofc;
	ofc.open("config.ini");
	ofc << "geth=" << toGethLoc << endl;
	ofc << "data=" << toDataLoc << endl;
	ofc.flush();
	ofc.close();
}

void auto_networkID()
{
    cout << "Auto Setup networkID as chainID " << chainID << endl;
    networkID.assign(chainID);
}

void auto_Nonce()
{
    char randNonce[16];
    for (int i = 0; i < 16; i++)
    {
        sprintf(randNonce + i, "%x", rand() % 16);
    }
    string newRandNonce(randNonce);
    nonce.assign(newRandNonce);
}

void auto_Difficulty()
{
    char randDiff[5];
    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
        {
            sprintf(randDiff + i, "%x", 1 + rand() % 9);
        }
        else
        {
            sprintf(randDiff + i, "%x", rand() % 10);
        }
    }
    string newRandDiff(randDiff);
    difficulty.assign(newRandDiff);
}

void auto_GasLimit()
{
    char randGas[8];
    for (int i = 0; i < 8; i++)
    {
        sprintf(randGas + i, "%x", rand() % 16);
    }
    string newRandGas(randGas);
    gas.assign(newRandGas);
}

