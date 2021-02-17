#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

string chainID;
string networkID;
string nonce;
string difficulty;
string gas;
string gethLoc;
string dataLoc;
bool isPoA = false;

int totalAccountCount = 0;
string *accounts;
string *passwords;
string *amounts;

vector<string> parameterStack;
int totalParametersCount = 0;

void ui_Interactive(); //Implemented
void ui_Unattended();
void ui_WriteConfig(); //Implemented
void ui_WriteConfigPoA(); //Implemented
void ui_Remove(); // Implemented
void ui_RemovePoA(); // Implemented
void ui_Create(); //Implemented
void ui_CreatePoA(); //Implemented
void ui_CreateAccount(); //Implemented
void ui_CreateAccountPoA(); //Implemented

bool check_chainID(); //Implemented
bool check_networkID(); //Implemented
bool check_nonce(); //Implemented
bool check_difficulty(); //Implemented
bool check_gasLimit(); //Implemented
bool check_chainExist(); //Implemented
bool check_chainExistPoA(); //Implemented
bool check_parStackWoreOut(); //Implemented

void config_Load(); //Implemented
void config_Reset(); //Implemented
void auto_networkID(); //Implemented
void auto_nonce(); //Implemented
void auto_difficulty(const int level); //Implemented
void auto_gasLimit(); //Implemented
void auto_autoAccount(string password, string preAlloc); //Implemented
string auto_password(const int digits); //Implemented

int main(int argc, char **argv)
{
	config_Load();
	if (argc < 2)
	{
		cout << "\n";
		cout << "Easy Chain Creation Tool v4 for Ethereum 1.9 / Linux" << "\n";
		cout << "Usage : createChain [-i/-c/-r] [chain ID] [Options]" << "\n";
		cout << "\t-i [Options...]\t\t\t Create a chain interactively." << "\n";
		cout << "\t-c [chain ID] [Options...]\t Create a chain, require Options." << "\n";
		cout << "\t-r [chain ID] [Options...]\t Remove a chain." << "\n";
		cout << "\n";
		cout << "[Options for -i (Interactively Create)] : " << "\n";
		cout << "\t-p \t\t\t <Optional> \t Create the chain with PoA Consensus Algorithm." << "\n";
		cout << "\n";
		cout << "[Options for -c (Create)] : " << "\n";
		cout << "\t[chain ID] \t\t <Mandatory> \t The chain ID for the new chain." << "\n";
		cout << "\n";
		cout << "\t-a [pw:am]... \t\t <Optional> \t Specifying the preconfigured accounts." << "\n";
		cout << "\t\t pw\t\t <Mandatory> \t Password of the account" << "\n";
		cout << "\t\t am\t\t <Mandatory> \t Prealloc amount for the account" << "\n";
		cout << "\t\t e.g., -a pass1:10000 pass2:20000" << "\n";
		cout << "\n";
		cout << "\t-b [num] [pw] [am] \t <Optional> \t Accounts batch creation." << "\n";
		cout << "\t\t num\t\t <Mandatory> \t The amount of accounts" << "\n";
		cout << "\t\t pw\t\t <Optional> \t The default password for accounts" << "\n";
		cout << "\t\t am\t\t <Optional> \t The default amount for accounts" << "\n";
		cout << "\t\t e.g., -b 3 pass 20000" << "\n";
		cout << "\n";
		cout << "\t-d [difficulty] \t <Optional> \t Determine the difficulty value for the chain." << "\n";
		cout << "\t\t difficulty \t <Mandatory> \t An unsigned decimal represents the initial mining difficulty." << "\n";
		cout << "\n";
		cout << "\t-g [gasLimit] \t\t <Optional> \t Determine the gas limit value for the chain." << "\n";
		cout << "\t\t gasLimit \t <Mandatory> \t An unsigned 8 digits hexical represents the maximum transaction fee allowed." << "\n";
		cout << "\n";
		cout << "\t-n [nonce] \t\t <Optional> \t Determine the nonce value for the chain." << "\n";
		cout << "\t\t nonce \t\t <Mandatory> \t An unsigned 16 digits hexical represents the nounce valud of the chain." << "\n";
		cout << "\n";
		cout << "\t-p \t\t\t <Optional> \t Create PoA chain." << "\n";
		cout << "\n";
		cout << "\t-w [networkID] \t\t <Optional> \t Determine the network ID for the chain." << "\n";
		cout << "\t\t networkID \t <Mandatory> \t An unsigned decimal represents the network ID of the chain." << "\n";
		cout << "\n";
		cout << "[Options for -r (Remove)] : " << "\n";
		cout << "\t[chain ID] \t\t <Mandatory> \t The chain you liked to remove." << "\n";
		cout << "\t-p \t\t\t <Optional> \t The chain to remove is a PoA chain." << "\n";
		cout << "\n";
		exit(1);

	}
	for (int i = argc -1; i > 0; i--)
	{
		string subParameter(argv[i]);
		
		/*Parameter Test*/
		//cout << "Paramter " << to_string(i) << " : " << subParameter << endl;

		parameterStack.push_back(subParameter);
	}
	
	/* Check the first action*/
	string identifier;
	identifier = parameterStack.back();
	parameterStack.pop_back();

	if (identifier == "-i")
	{
		do
		{
			if (check_parStackWoreOut())
			{
				break;
			}
			string subIdentifier = parameterStack.back();
			parameterStack.pop_back();
			if (subIdentifier == "-p")
			{
				isPoA = true;
			}
		} while (parameterStack.size() > 0);
		ui_Interactive();
	}
	else if (identifier == "-c")
	{
		if (check_parStackWoreOut())
		{
			cout << "Missing critical arguments for -c" << "\n";
			exit(1);
		}
		chainID = parameterStack.back();
		parameterStack.pop_back();

		if (!check_chainID)
		{
			cout << "You provided an incorrect chainID." << "\n";
			exit(1);
		}

		ui_Unattended();
	}
	else if (identifier == "-r")
	{
		if (check_parStackWoreOut())
		{
			cout << "Missing critical arguments for -r.\n";
			exit(1);
		}

		chainID = parameterStack.back();
		parameterStack.pop_back();

		if (!check_chainID)
		{
			cout << "You provided an incorrect chainID." << "\n";
			exit(1);
		}

		do
		{
			if (check_parStackWoreOut())
			{
				break;
			}
			string subIdentifier = parameterStack.back();
			parameterStack.pop_back();
			if (subIdentifier == "-p")
			{
				isPoA = true;
			}
		} while (parameterStack.size() > 0);

		if (isPoA)
		{
			if (!check_chainExistPoA())
			{
				cout << "Chain " << chainID << " does not exist" << "\n";
				exit(1);
			}
			ui_RemovePoA();
		}
		else
		{
			if (!check_chainExist())
			{
				cout << "Chain " << chainID << " does not exist" << "\n";
				exit(1);
			}
			ui_Remove();
		}

	}
	else
	{
		cout << "The first parameter or action cannot be identified.\n";
		exit(1);
	}
}

bool check_parStackWoreOut()
{
	if (parameterStack.size() < 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ui_Unattended()
{
	do
	{
		if (check_parStackWoreOut())
		{
			break;
		}
		string identifier = parameterStack.back();
		parameterStack.pop_back();
		
		if (identifier == "-a")
		{
			if (check_parStackWoreOut())
			{
				cout << "Missing critical argument for -a" << "\n";
				exit(1);
			}

			if (totalAccountCount != 0)
			{
				cout << "Please do not respecify accounts.\n";
				exit(1);
			}

			vector<string> tempAccounts;
			bool endOfAccount = false;
			bool isIdentifier = false;
			string identifiers[] = {"-d", "-g", "-n", "-p", "-w"};

			while (!check_parStackWoreOut() && !isIdentifier)
			{
				for (int i = 0; i < 5; i++)
				{
					if (parameterStack.back() == identifiers[i])
					{
						isIdentifier = true;
					}
				}
				if (!isIdentifier)
				{
					if ((parameterStack.back()).find(":") == std::string::npos)
					{
						cout << "This is not a correct format of representing an account." << "\n";
						exit(1);
					}
					else
					{
						tempAccounts.push_back(parameterStack.back());
						parameterStack.pop_back();
					}
				}
			}

			totalAccountCount = tempAccounts.size();
			passwords = new string[totalAccountCount];
			amounts = new string[totalAccountCount];

			for (int i = 0; i < totalAccountCount; i++)
			{
				string oneAccount = tempAccounts.back();
				std::istringstream ss(oneAccount);
				tempAccounts.pop_back();
				string token;
				getline(ss, token, ':');
				passwords[i] = token;
				getline(ss, token, ':');
				amounts[i] = token;
			}



		}
		else if (identifier == "-b")
		{
			if (totalAccountCount != 0)
			{
				cout << "Please do not respecify accounts.\n";
				exit(1);
			}

			bool noAccPassword = false;
			bool noAccAmount = false;
			string accPassword = "";
			string accAmount = "";

			if (check_parStackWoreOut())
			{
				cout << "Missing critical arguments for -b" << "\n";
				exit(1);
			}
			string accNumberString = parameterStack.back();
			parameterStack.pop_back();
			if (accNumberString.at(0) == '0' || accNumberString.find_first_not_of("1234567890") != std::string::npos)
			{
				cout << "The amount of account should be a number !";
				exit(1);
			}
			else
			{
				totalAccountCount = std::stoi(accNumberString);
				string identifiers[] = {"-d", "-g", "-n", "-p", "-w"};
				
				if (check_parStackWoreOut())
				{
					noAccPassword = true;
				}
				else
				{
					for (int i = 0; i < 5; i++)
					{
						if (parameterStack.back() == identifiers[i])
						{
							noAccPassword = true;
						}
					}
				}

				if (noAccPassword)
				{
					auto_autoAccount("", "");
				}
				else
				{
					accPassword = parameterStack.back();
					parameterStack.pop_back();

					if (check_parStackWoreOut())
					{
						noAccAmount = true;
					}
					else
					{
						for (int i = 0; i < 5; i++)
						{
							if (parameterStack.back() == identifiers[i])
							{
								noAccAmount = true;
							}

						}
					}
					if (noAccAmount)
					{
						auto_autoAccount(accPassword, "");
					}
					else
					{
						accAmount = parameterStack.back();
						parameterStack.pop_back();
						if (accAmount.at(0) == '0' || accAmount.find_first_not_of("1234567890") != std::string::npos)
						{
							cout << "You provided a bad amount" << "\n";
							exit(1);
						}
						else
						{
							auto_autoAccount(accPassword, accAmount);
						}
					}

				}

			}

		}
		else if (identifier == "-d")
		{
			if (check_parStackWoreOut())
			{
				cout << "Missing critical argument for -d" << "\n";
				exit(1);
			}
			if (!difficulty.empty())
			{
				cout << "Please do not respecify difficulty.\n";
				exit(1);
			}
			difficulty = parameterStack.back();
			parameterStack.pop_back();
			if (!check_difficulty())
			{
				cout << "Bad Difficulty." << "\n";
				exit(1);
			}
		}
		else if (identifier == "-g")
		{
			if (check_parStackWoreOut())
			{
				cout << "Missing critical argument for -g" << "\n";
				exit(1);
			}
			if (!gas.empty())
			{
				cout << "Please do not respecify gas limit.\n";
				exit(1);
			}
			gas = parameterStack.back();
			parameterStack.pop_back();
			if (!check_gasLimit())
			{
				cout << "Bad Gas Limit." << "\n";
				exit(1);
			}
		}
		else if (identifier == "-n")
		{
			if (check_parStackWoreOut())
			{
				cout << "Missing critical argument for -n" << "\n";
				exit(1);
			}
			if (!nonce.empty())
			{
				cout << "Please do not respecify nonce.\n";
				exit(1);
			}
			nonce = parameterStack.back();
			parameterStack.pop_back();
			if (!check_nonce())
			{
				cout << "Bad Nonce Value." << "\n";
				exit(1);
			}
		}
		else if (identifier == "-p")
		{
			if (isPoA)
			{
				cout << "Please do not respecify PoA.\n";
				exit(1);
			}
			isPoA = true;
		}
		else if (identifier == "-w")
		{
			if (check_parStackWoreOut())
			{
				cout << "Missing critical argument for -w" << "\n";
				exit(1);
			}
			if (!networkID.empty())
			{
				cout << "Please do not respecify networkID.\n";
				exit(1);
			}
			networkID = parameterStack.back();
			parameterStack.pop_back();
			if (!check_networkID())
			{
				cout << "Bad network ID." << "\n";
				exit(1);
			}
		}
		else
		{
			cout << "Bad parameters." << "\n";
			exit(1);
		}

	} while (parameterStack.size() > 0);

	// Check the integrity of all parameters //
	if (networkID.empty())
	{
		auto_networkID();
	}
	if (nonce.empty())
	{
		auto_nonce();
	}
	if (difficulty.empty())
	{
		auto_difficulty(1);
	}
	if (gas.empty())
	{
		auto_gasLimit();
	}


	if (isPoA)
	{
		ui_WriteConfigPoA();
	}
	else
	{
		ui_WriteConfig();
	}
}

void ui_WriteConfigPoA()
{
	if (totalAccountCount < 2)
	{
		cout << "It is required to create at least 2 accounts to enable PoA\n";
		exit(1);
	}
	ui_CreatePoA();
	ui_CreateAccountPoA();

	for (int i = 0; i < totalAccountCount; i++)
	{
		ofstream genesisFile;
		string gFFilePath = "";
		gFFilePath.append(dataLoc);
		if (dataLoc.at(dataLoc.length() -1) != '/')
		{
			gFFilePath.append("/");
		}
		gFFilePath.append("chain-");
		gFFilePath.append(chainID);
		gFFilePath.append("-P");
		gFFilePath.append(to_string(i));
		gFFilePath.append("/genesis.json");

		genesisFile.open(gFFilePath);
		genesisFile << "{" << "\n";
		genesisFile << "\t" << "\"config\": {" << "\n";
		genesisFile << "\t\t" << "\"chainId\": " << chainID.c_str() << "," << "\n";
		genesisFile << "\t\t" << "\"homesteadBlock\": 0," << "\n";
		genesisFile << "\t\t" << "\"eip150Block\": 0," << "\n";
		genesisFile << "\t\t" << "\"eip155Block\": 0," << "\n";
		genesisFile << "\t\t" << "\"eip158Block\": 0," << "\n";
		genesisFile << "\t\t" << "\"byzantiumBlock\": 0," << "\n";
		genesisFile << "\t\t" << "\"constantinopleBlock\": 0," << "\n";
		genesisFile << "\t\t" << "\"petersburgBlock\": 0," << "\n";
		genesisFile << "\t\t" << "\"istanbulBlock\": 0" << "\n";
		genesisFile << "\t" << "}," << "\n";
		genesisFile << "\t" << "\"nonce\": \"0x" << nonce.c_str() << "\"," << "\n";
		genesisFile << "\t" << "\"timestamp\": \"0x0\"," << "\n";
		//ExtraData Config
		genesisFile << "\t" << "\"extraData\": \"0x";
		for (int j = 0; j < 64; j++)
		{
			genesisFile << "0";
		}
		for (int j = 0; j < totalAccountCount; j++)
		{
			genesisFile << (accounts[j]).substr(2, 40);
		}
		for (int j = 0; j < 130; j++)
		{
			genesisFile << "0";
		}
		genesisFile << "\",\n";


		//End of ExtraData Config
		
		genesisFile << "\t" << "\"difficulty\": \"0x" << difficulty.c_str() << "\"," << "\n";
		genesisFile << "\t" << "\"gasLimit\": \"0x" << gas.c_str() << "\"," << "\n";
		genesisFile << "\t" << "\"mixhash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\"," << "\n";
		genesisFile << "\t" << "\"parentHash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\"," << "\n";
		genesisFile << "\t" << "\"clique\": {" << "\n";
		genesisFile << "\t\t" << "\"period\": 15," << "\n";
		genesisFile << "\t\t" << "\"epoch\": 30000" << "\n";
		genesisFile << "\t" << "}," << "\n";
		genesisFile << "\t" << "\"alloc\": {" << "\n";
		for (int j = 0; j < totalAccountCount; j++)
		{
			genesisFile << "\t\t" << "\"" << (accounts[j]).substr(2, 40) << "\": {\n";
			genesisFile << "\t\t\t" << "\"balance\": \"" << amounts[j] << "\"\n";
			genesisFile << "\t\t}";
			if (j == totalAccountCount -1)
			{
				genesisFile << "\n";
			}
			else
			{
				genesisFile << ",\n";
			}
		}
		genesisFile << "\t}\n";
		genesisFile << "}";
		genesisFile.flush();
		genesisFile.close();

		ofstream bootScript;
		string bSFilePath = "";
		bSFilePath.append(dataLoc);
		if (dataLoc.at(dataLoc.length() -1) != '/')
		{
			bSFilePath.append("/");
		}
		bSFilePath.append("chain-");
		bSFilePath.append(chainID);
		bSFilePath.append("-P");
		bSFilePath.append(to_string(i));
		bSFilePath.append("/boot.sh");
		bootScript.open(bSFilePath);
		bootScript << gethLoc.c_str() << " console --datadir " << dataLoc.c_str();
		if (dataLoc.at(dataLoc.length() -1) != '/')
		{
			bootScript << '/';
		}
		bootScript << "chain-" << chainID.c_str() << "-P" << to_string(i) << "/";
		bootScript << " --networkid " << networkID.c_str();
		bootScript << " --port " << to_string(30303 + i);
		bootScript << " --nodiscover" << "\n";
		bootScript.flush();
		bootScript.close();

		string bSModCmd = "chmod 775 ";
		bSModCmd.append(bSFilePath);
		system(bSModCmd.c_str());

		ofstream initScript;
		string iSFilePath = "";
		iSFilePath.append(dataLoc);
		if (dataLoc.at(dataLoc.length() -1) != '/')
		{
			iSFilePath.append("/");
		}
		iSFilePath.append("chain-");
		iSFilePath.append(chainID);
		iSFilePath.append("-P");
		iSFilePath.append(to_string(i));
		iSFilePath.append("/init.sh");
		initScript.open(iSFilePath);
		initScript << gethLoc.c_str() << " --datadir " << dataLoc.c_str();
		if (dataLoc.at(dataLoc.length() -1) != '/')
		{
			initScript << "/";
		}
		initScript << "chain-" << chainID.c_str() << "-P" << to_string(i) << "/ init ";
		initScript << dataLoc.c_str();
		if (dataLoc.at(dataLoc.length() -1) != '/')
		{
			initScript << "/";
		}
		initScript << "chain-" << chainID.c_str() << "-P" << to_string(i) << "/genesis.json" << "\n";
		initScript.flush();
		initScript.close();

		string iSModCmd = "chmod 775 ";
		iSModCmd.append(iSFilePath);
		system(iSModCmd.c_str());
	}
	system("clear");
	cout << "Quick Configuration Review: \n";
	cout << "Chain ID : \t\t" << chainID << "\n";
	cout << "Network ID : \t\t" << networkID << "\n";
	cout << "Nonce : \t\t" << nonce << "\n";
	cout << "GasLimit : \t\t" << gas << "\n";
	cout << "Difficulty : \t\t" << difficulty << "\n";
	cout << "Consensus : \t\tClique (PoA)" << "\n";
	cout << "Datastore : \t\t" << dataLoc;
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		cout << "/";
	}
	cout << "chain-" << chainID << "-P*" << "/\n";
	cout << "\n";
	cout << "Preconfigured Authority Accounts \n";
	for (int i = 0; i < totalAccountCount; i++)
	{
		cout << "P" << i << ":\t" << accounts[i] << "\n";
		cout << "\t\tPasswords:\t" << passwords[i] << "\n";
		cout << "\t\tPrealloc:\t" << amounts[i] << "\n";
		cout << "\t\tPort:\t" << to_string(30303 + i) << "\n";
	}

	cout << "\n";

	cout << "In order to emulate multinode environment,\n";
	cout << "we seperate authority accounts into their own datastore respectively.\n";
	cout << "Use init.sh to init authority nodes respectively, run ONCE in each. \n";
	cout << "Use boot.sh to boot authority nodes respectively, AFTER init. \n";

	cout << "\n";

	cout << "To prevent the potential security issues to your testing environment,\n";
	cout << "Auto-Discovery is turned off by default. \n";
	cout << "You can either remove the --nodiscover option in the boot script file,\n";
	cout << "Or manually adding nodes.\n";

	cout << "\n";

	cout << "Please ensure there are 2 or more authority nodes online. \n";

}
void ui_WriteConfig()
{
	ui_Create();
	if (totalAccountCount > 0)
	{
		ui_CreateAccount();
	}
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
	genesisFile << "\t\t" << "\"eip158Block\": 0," << "\n";
	genesisFile << "\t\t" << "\"byzantiumBlock\": 0," << "\n";
	genesisFile << "\t\t" << "\"constantinopleBlock\": 0," << "\n";
	genesisFile << "\t\t" << "\"petersburgBlock\": 0," << "\n";
	genesisFile << "\t\t" << "\"istanbulBlock\": 0" << "\n";
	genesisFile << "\t" << "}," << "\n";
	genesisFile << "\t" << "\"nonce\": \"0x" << nonce.c_str() << "\"," << "\n";
	genesisFile << "\t" << "\"timestamp\": \"0x0\"," << "\n";
	genesisFile << "\t" << "\"extraData\": \"0x\"," << "\n";
	genesisFile << "\t" << "\"difficulty\": \"0x" << difficulty.c_str() << "\"," << "\n";
	genesisFile << "\t" << "\"gasLimit\": \"0x" << gas.c_str() << "\"," << "\n";
	genesisFile << "\t" << "\"mixhash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\"," << "\n";
	genesisFile << "\t" << "\"parentHash\": \"0x0000000000000000000000000000000000000000000000000000000000000000\"," << "\n";
	if (totalAccountCount > 0)
	{
		genesisFile << "\t" << "\"alloc\": {" << "\n";
		for (int i = 0; i < totalAccountCount; i++)
		{
			genesisFile << "\t\t" << "\"" << (accounts[i]).substr(2, 40) << "\": {\n";
			genesisFile << "\t\t\t" << "\"balance\": \"" << amounts[i] << "\"\n";
			genesisFile << "\t\t}";
			if (i == totalAccountCount -1)
			{
				genesisFile << "\n";
			}
			else
			{
				genesisFile << ",\n";
			}

		}
		genesisFile << "\t}\n";
	}
	else
	{
		genesisFile << "\t" << "\"alloc\": {}" << "\n";
	}
	genesisFile << "}";
	genesisFile.flush();
	genesisFile.close();

	ofstream bootScript;
	string bSFilePath = "";
	bSFilePath.append(dataLoc);
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		bSFilePath.append("/");
	}
	bSFilePath.append("chain-");
	bSFilePath.append(chainID);
	bSFilePath.append("/boot.sh");
	bootScript.open(bSFilePath);
	bootScript << gethLoc.c_str() << " console --datadir " << dataLoc.c_str();
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		bootScript << '/';
	}
	bootScript << "chain-" << chainID.c_str() << "/ --networkid " << networkID.c_str() << " --port 30303 --nodiscover" << "\n";
	bootScript.flush();
	bootScript.close();

	string bSModCmd = "chmod 775 ";
	bSModCmd.append(bSFilePath);
	system(bSModCmd.c_str());

	ofstream initScript;
	string iSFilePath = "";
	iSFilePath.append(dataLoc);
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		iSFilePath.append("/");
	}
	iSFilePath.append("chain-");
	iSFilePath.append(chainID);
	iSFilePath.append("/init.sh");
	initScript.open(iSFilePath);
	initScript << gethLoc.c_str() << " --datadir " << dataLoc.c_str();
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		initScript << "/";
	}
	initScript << "chain-" << chainID.c_str() << "/ init ";
	initScript << dataLoc.c_str();
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		initScript << "/";
	}
	initScript << "chain-" << chainID.c_str() << "/genesis.json" << "\n";
	initScript.flush();
	initScript.close();

	string iSModCmd = "chmod 775 ";
	iSModCmd.append(iSFilePath);
	system(iSModCmd.c_str());

	system("clear");
	cout << "Quick Configuration Review: \n";
	cout << "Chain ID : \t\t" << chainID << "\n";
	cout << "Network ID : \t\t" << networkID << "\n";
	cout << "Nonce : \t\t" << nonce << "\n";
	cout << "GasLimit : \t\t" << gas << "\n";
	cout << "Difficulty : \t\t" << difficulty << "\n";
	cout << "Consensus : \t\tetHash (PoW)" << "\n";
	cout << "Datastore : \t\t" << dataLoc;
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		cout << "/";
	}
	cout << "chain-" << chainID << "/\n";

	cout << "\n";
	if (totalAccountCount > 0)
	{
		cout << "Preconfigured Accounts \n";
		for (int i = 0; i < totalAccountCount; i++)
		{
			cout << i << ":\t" << accounts[i] << "\n";
			cout << "\t\tPasswords:\t" << passwords[i] << "\n";
			cout << "\t\tPrealloc:\t" << amounts[i] << "\n";
		}
	}
	else
	{
		cout << "No Preconfigured Accounts \n";
	}
	
	cout << "\n";	
	cout << "You MUST initialize your private chain before you use it\n";
	cout << "Use " << iSFilePath << " to init your private chain, run ONCE.\n";
	cout << "Use " << bSFilePath << " to boot your private chain, AFTER init.\n";
	
}

void ui_RemovePoA()
{
	string command = "rm -rf ";
	command.append(dataLoc);
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		command.append("/");
	}
	command.append("chain-");
	command.append(chainID);
	command.append("-P*");
	system(command.c_str());
}

void ui_Remove()
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

void ui_CreateAccountPoA()
{
	if (totalAccountCount < 2)
	{
		cout << "It is required to have at least 2 accounts to enable PoA";
		exit(1);
	}
	accounts = new string[totalAccountCount];
	for (int i = 0; i < totalAccountCount; i++)
	{
		ofstream ofc;
		ofc.open("password.txt");
		ofc << passwords[i];
		ofc.flush();
		ofc.close();

		string command = gethLoc;
		command.append(" account new --datadir ");
		command.append(dataLoc);
		if (dataLoc.at(dataLoc.length() -1) != '/')
		{
			command.append("/");

		}
		command.append("chain-");
		command.append(chainID);
		command.append("-P");
		command.append(to_string(i));
		command.append(" -password ./password.txt");
		command.append(" >> result.txt");
		system(command.c_str());

		ifstream ifc;
		ifc.open("result.txt");
		if (ifc.good())
		{
			while (ifc.peek() != EOF)
			{
				string temp;
				getline(ifc, temp);
				if (temp.find("0x") != std::string::npos)
				{
					accounts[i] = temp.substr(temp.find("0x"), 42);
				}
			}
			ifc.close();
			system("rm -rf ./result.txt");
		}
		else
		{
			ifc.close();
			cout << "Cannot read the output result from geth \n";
			system("rm -rf ./result.txt");
		}
		system("rm -rf ./password.txt");

	}
	
}

void ui_CreateAccount()
{
	accounts = new string[totalAccountCount];
	for (int i = 0; i < totalAccountCount; i++)
	{
		ofstream ofc;
		ofc.open("password.txt");
		ofc << passwords[i];
		ofc.flush();
		ofc.close();

		string command = gethLoc;
		command.append(" account new --datadir ");
		command.append(dataLoc);
		if (dataLoc.at(dataLoc.length() -1) != '/')
		{
			command.append("/");
		}
		command.append("chain-");
		command.append(chainID);
		command.append(" -password ./password.txt");
		command.append(" >> result.txt");
		system(command.c_str());

		ifstream ifc;
		ifc.open("result.txt");
		if (ifc.good())
		{
			while (ifc.peek() != EOF)
			{
				string temp;
				getline(ifc, temp);
				if (temp.find("0x") != std::string::npos)
				{
					accounts[i] = temp.substr(temp.find("0x"),42);
				}
			}
			ifc.close();
			system("rm -rf ./result.txt");
		}
		else
		{
			ifc.close();
			cout << "Cannot read the output result from geth \n";
			system("rm -rf ./result.txt");
			exit(1);
		}
		system("rm -rf ./password.txt");

	}

}

void ui_CreatePoA()
{
	if (check_chainExistPoA())
	{
		cout << "PoA Chain " << chainID << " Exists! \n";
		exit(1);
	}
	if (totalAccountCount < 2)
	{
		cout << "It is required to create at least 2 accounts to enable PoA!\n";
		exit(1);
	}
	else
	{
		for (int i = 0; i < totalAccountCount; i++)
		{
			string command = "mkdir ";
			command.append(dataLoc);
			if (dataLoc.at(dataLoc.length() -1) != '/')
			{
				command.append("/");
			}
			command.append("chain-");
			command.append(chainID);
			command.append("-P");
			command.append(to_string(i));
			system(command.c_str());
		}
	}
}

void ui_Create()
{
	if (check_chainExist())
	{
		cout << "Chain " << chainID << " Exists! \n";
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

void ui_Interactive()
{
	do
	{
		cout << "Enter preferred chain ID \n";
		getline(cin, chainID);
	} while (!check_chainID());

	do
	{
		cout << "Enter preferred network ID. Enter X for auto setup \n";
		getline(cin, networkID);
		if (networkID.compare("X") == 0)
		{
			auto_networkID();
		}
	} while (!check_networkID());

	do
	{
		cout << "Enter preferred Nonce. Enter X for auto setup \n";
		getline(cin, nonce);
		if (nonce.compare("X") == 0)
		{
			auto_nonce();
			cout << "Nonce Generated as " << nonce << "\n";
		}
	} while (!check_nonce());

	do
	{
		cout << "Enter preffered GasLimit. Enter X for auto setup \n";
		getline(cin, gas);
		if (gas.compare("X") == 0)
		{
			auto_gasLimit();
			cout << "GasLimit Generated as " << gas << "\n";
		}
		
	} while (!check_gasLimit());

	do
	{
		cout << "Enter preffered Difficulty. Enter X for auto setup \n";
		getline(cin, difficulty);
		if (difficulty.compare("X") == 0)
		{
			cout << "Please determine the preffered difficulty level \n";
			cout << "\t0: Lowest\n";
			cout << "\t1: Low\n";
			cout << "\t2: Medium\n";
			cout << "\t3: High\n";
			string choice = "";
			do
			{
				getline(cin, choice);
			} while ( choice.length() != 1 && choice.find_first_not_of("0123") == std::string::npos);
			auto_difficulty(atoi(choice.c_str()));
		}
	} while (!check_difficulty());
	
	string accountInput;
	do
	{
		cout << "Do you want to create accounts (Y/N)? \n";
		getline(cin, accountInput);

	} while (accountInput.length() == 1 && accountInput.find_first_not_of("YNyn") != std::string::npos);

	if (accountInput == "Y" || accountInput == "y")
	{
		string accountAmount;
		do
		{
			cout << "How many accounts you would like to create ? \n";
			getline(cin, accountAmount);
		} while (accountAmount.at(0) == '0' || accountAmount.find_first_not_of("0123456789") != std::string::npos);
		
		string accountPrealloc;
		string accountPreallocInput = "";

		do
		{
			cout << "Do you want to prealloc funds for all accounts (Y/N)? \n";
			getline(cin, accountPrealloc);

		} while (accountPrealloc.length() == 1 && accountPrealloc.find_first_not_of("YyNn") != std::string::npos);

		if (accountPrealloc == "Y" || accountPrealloc == "y")
		{
			do
			{
				cout << "Enter the Prealloc Amount in Wei" << endl;
				getline(cin, accountPreallocInput);
			} while (accountPreallocInput.at(0) == '0' || accountPreallocInput.find_first_not_of("1234567890") != std::string::npos);

		}	

		string accountPassword;
		string accountPasswordInput = "";
		do
		{
			cout << "Do you want to create pre-set password for all accounts (Y/N)? \n";
			getline(cin, accountPassword);
		} while (accountPassword.length() == 1 && accountPassword.find_first_not_of("YyNn") != std::string::npos);
		
		if (accountPassword == "Y" || accountPassword == "y")
		{
			do
			{
				cout << "Enter pre-set password here. \n";
				getline(cin, accountPasswordInput);
			} while (accountPasswordInput.find_first_not_of("1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") != std::string::npos);	
		}
		totalAccountCount = std::stoi(accountAmount);
		auto_autoAccount(accountPasswordInput, accountPreallocInput);
	}
	if (isPoA)
	{
		ui_WriteConfigPoA();

	}
	else
	{
		ui_WriteConfig();
	}
}


bool check_chainID()
{
	if (chainID.length() > 5)
	{
		cout << "Unsupported Chain ID \n";
		return false;
	}
	else if (chainID.length() < 2)
	{
		cout << "To prevent collision, DO NOT assign a chain ID fewer than 2 digits \n";
		return false;
	}
	else if (chainID.at(0) == '0')
	{
		cout << "chainID shuold not start with 0 \n";
		return false;
	}
	return chainID.find_first_not_of("0123456789") == std::string::npos;
}

bool check_networkID()
{
	if (networkID.length() > 5)
	{
		cout << "Unsupported Network ID \n";
		return false;
	}
	else if (networkID.length() < 2)
	{
		cout << "To prevent collision, DO NOT assign a network ID fewer than 2 digits \n";
		return false;
	}
	else if (networkID.at(0) == '0')
	{
		cout << "networkID should not start with 0 \n";
		return false;
	}
	return networkID.find_first_not_of("0123456789") == std::string::npos;
}

bool check_nonce()
{
	return nonce.find_first_not_of("0123456789abcdef") == std::string::npos;
}

bool check_difficulty()
{
	if (difficulty.at(0) == '0')
	{
		cout << "Difficulty should not starts with 0. \n";
		return false;
	}
	if (difficulty.length() > 6)
	{
		cout << "Please note that setting high difficulty value may resulted in low mining performance, \n";
	}
	return difficulty.find_first_not_of("0123456789abcdef") == std::string::npos;
}

bool check_gasLimit()
{
	if (gas.at(0) == '0')
	{
		cout << "Gas limit should not starts with 0. \n";
		return false;
	}
	if (gas.length() > 8)
	{
		cout << "You assigned a gasLimit that is over the system can allow. \n";
		return false;
	}
	return gas.find_first_not_of("0123456789abcdef") == std::string::npos;
}

bool check_chainExist()
{
	struct stat buffer;
	string folder = "";
	folder.append(dataLoc);
	folder.append("chain-");
	folder.append(chainID);
	return (stat (folder.c_str(), &buffer) == 0);
}

bool check_chainExistPoA()
{
	struct stat buffer;
	string folder = "";
	folder.append(dataLoc);
	folder.append("chain-");
	folder.append(chainID);
	folder.append("-P0");
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
			cout << "Bad config file! \n";
			config_Reset();
		}
	}
	else
	{
		ifc.close();
		cout << "No config.ini file! \n";
		config_Reset();
	}
}

void config_Reset()
{
	cout << "The location of geth executable ? \n";
	string toGethLoc;
	getline(cin, toGethLoc);
	gethLoc.assign(toGethLoc);
	cout << "The location to store chain data ? \n";
	string toDataLoc;
	getline(cin, toDataLoc);
	dataLoc.assign(toDataLoc);
	cout << "Config Reset! \n";
	ofstream ofc;
	ofc.open("config.ini");
	ofc << "geth=" << toGethLoc << "\n";
	ofc << "data=" << toDataLoc << "\n";
	ofc.flush();
	ofc.close();
}

void auto_networkID()
{
	cout << "Auto Setup networkID as chainID " << chainID << "\n";
	networkID.assign(chainID);
}

void auto_nonce()
{
	srand(time(NULL));
	char randNonce[16];
	for (int i = 0; i < 16; i++)
	{
		sprintf(randNonce + i, "%x", rand() % 16);
	}
	string newRandNonce(randNonce);
	nonce.assign(newRandNonce);
}

void auto_difficulty(const int level)
{
	if (level == 0)
	{
		cout << "Lowest difficulty level \n";
		cout << "Please use this only in an experimental environment \n";
		difficulty.assign("1");
	}
	else if (level > 0 && level <= 3)
	{
		string naming[] = {"Low difficulty level", "Medium difficulty level", "High difficulty level"};
		cout << naming[level - 1] << "\n";
		srand(time(NULL));
		char randDiff[level + 2];
		for (int i = 0; i < level + 2; i++)
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
	else
	{
		cout << "Unrecognized difficulty level \n";
	}
}

void auto_gasLimit()
{
	cout << "Auto Setup gasLimit as Maximum Allowance \n";
	gas.assign("ffffffff");
}

void auto_autoAccount(string password, string preAlloc)
{
	if (totalAccountCount < 1)
	{
		cout << "Something went wrong when specifying the amount of number \n";
		exit(1);
	}
	else if (totalAccountCount > 100)
	{
		cout << "Warning! Specifying a great number of accounts. \n";
	}
	if (password.empty())
	{
		cout << "Not specifying a password, auto-generate for each. \n";
		passwords = new string[totalAccountCount];
		for (int i = 0; i < totalAccountCount; i++)
		{
			string genPassword = auto_password(6);
			passwords[i] = genPassword;
		}

	}
	else
	{
		cout << "You specified a password for auto account create. \n";
		passwords = new string[totalAccountCount];
		for (int i = 0; i < totalAccountCount; i++)
		{
			passwords[i] = password;
		}
	}

	if (preAlloc.empty())
	{
		cout << "You did not specified how much to pre-allocate to each account. \n";
		cout << "Assumed 0 Wei\n";
		amounts = new string[totalAccountCount];
		for (int i = 0; i < totalAccountCount; i++)
		{
			amounts[i] = "0";
		}
	}
	else
	{
		if (preAlloc.at(0) == '0')
		{
			cout << "Pre-allocate should not start with 0. \n";
			exit(1);
		}
		else if (preAlloc.find_first_not_of("0123456789") != std::string::npos)
		{
			cout << "You specified a bad pre-allocate value. \n";
			exit(1);
		}
		else
		{
			amounts = new string[totalAccountCount];
			for (int i = 0; i < totalAccountCount; i++)
			{
				amounts[i] = preAlloc;
			}
		}
	}
}

string auto_password(const int digits)
{
	sleep(1);
	string sample = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	string returnPassword;
	srand(time(NULL));
	for (int i = 0; i < digits; i++)
	{
		int choice = rand() % sample.length();
		returnPassword.append(sample.substr(choice, 1));
	}
	return returnPassword;
}
