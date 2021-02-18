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
		cout << "Ethereum 私有鏈建立工具 v4 / Linux平台" << "\n";
		cout << "使用方法 : createChain [-i/-c/-r] [chain ID] [Options]" << "\n";
		cout << "\t-i [Options...]\t\t\t 互動式建立" << "\n";
		cout << "\t-c [chain ID] [Options...]\t 建立私有鏈，需要參數" << "\n";
		cout << "\t-r [chain ID] [Options...]\t 移除私有鏈" << "\n";
		cout << "\n";
		cout << "[-i (互動式建立) 適用的選項] : " << "\n";
		cout << "\t-p \t\t\t <可選> \t 建立的私有鏈使用 PoA 共識演算法" << "\n";
		cout << "\n";
		cout << "[-c (建立私有鏈) 適用的選項] : " << "\n";
		cout << "\t[chain ID] \t\t <必須> \t 私有鏈的 Chain ID" << "\n";
		cout << "\n";
		cout << "\t-a [pw:am]... \t\t <可選> \t 預先設定帳號資訊" << "\n";
		cout << "\t\t pw\t\t <必須> \t 帳戶的密碼" << "\n";
		cout << "\t\t am\t\t <必須> \t 帳戶的預存金額" << "\n";
		cout << "\t\t 範例 -a pass1:10000 pass2:20000" << "\n";
		cout << "\n";
		cout << "\t-b [num] [pw] [am] \t <可選> \t 大量帳號建立工具" << "\n";
		cout << "\t\t num\t\t <必須> \t 建立的帳號數量" << "\n";
		cout << "\t\t pw\t\t <可選> \t 帳號的預設密碼" << "\n";
		cout << "\t\t am\t\t <可選> \t 帳號的預設預存金額" << "\n";
		cout << "\t\t 範例 -b 3 pass 20000" << "\n";
		cout << "\n";
		cout << "\t-d [difficulty] \t <可選> \t 設定私有鏈的挖礦難度" << "\n";
		cout << "\t\t difficulty \t <必須> \t 一組代表挖礦難度的十進位數字" << "\n";
		cout << "\n";
		cout << "\t-g [gasLimit] \t\t <可選> \t 設定交易手續費上限" << "\n";
		cout << "\t\t gasLimit \t <必須> \t 一組代表交易手續費上限的 8 位數十六進位數字" << "\n";
		cout << "\n";
		cout << "\t-n [nonce] \t\t <可選> \t 設定私有鏈的亂數值" << "\n";
		cout << "\t\t nonce \t\t <必須> \t 一組代表亂數值的 16 位數十六進位數字" << "\n";
		cout << "\n";
		cout << "\t-p \t\t\t <可選> \t 建立的私有鏈使用 PoA 共識演算法" << "\n";
		cout << "\n";
		cout << "\t-w [networkID] \t\t <可選> \t 設定私有鏈的 network ID" << "\n";
		cout << "\t\t networkID \t <必須> \t 一組代表 network ID 的十進位數字" << "\n";
		cout << "\n";
		cout << "[-r (移除) 適用的選項] : " << "\n";
		cout << "\t[chain ID] \t\t <必須> \t 要移除的私有鏈的 Chain ID" << "\n";
		cout << "\t-p \t\t\t <可選> \t 要移除的私有鏈使用 PoA 共識演算法" << "\n";
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
			cout << "參數不足，無法執行" << "\n";
			exit(1);
		}
		chainID = parameterStack.back();
		parameterStack.pop_back();

		if (!check_chainID)
		{
			cout << "您輸入了不正確的 chainID" << "\n";
			exit(1);
		}

		ui_Unattended();
	}
	else if (identifier == "-r")
	{
		if (check_parStackWoreOut())
		{
			cout << "參數不足，無法執行\n";
			exit(1);
		}

		chainID = parameterStack.back();
		parameterStack.pop_back();

		if (!check_chainID)
		{
			cout << "您輸入了不正確的 chainID" << "\n";
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
				cout << "找不到 " << chainID << " 私有鏈" << "\n";
				exit(1);
			}
			ui_RemovePoA();
		}
		else
		{
			if (!check_chainExist())
			{
				cout << "找不到 " << chainID << " 私有鏈" << "\n";
				exit(1);
			}
			ui_Remove();
		}

	}
	else
	{
		cout << "您指定了錯誤的使用方法\n";
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
				cout << "參數不足，無法執行" << "\n";
				exit(1);
			}

			if (totalAccountCount != 0)
			{
				cout << "請不要重新設定帳戶\n";
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
						cout << "您所提供的帳戶資訊格式錯誤" << "\n";
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
				cout << "請不要重新設定帳戶\n";
				exit(1);
			}

			bool noAccPassword = false;
			bool noAccAmount = false;
			string accPassword = "";
			string accAmount = "";

			if (check_parStackWoreOut())
			{
				cout << "參數不足，無法執行" << "\n";
				exit(1);
			}
			string accNumberString = parameterStack.back();
			parameterStack.pop_back();
			if (accNumberString.at(0) == '0' || accNumberString.find_first_not_of("1234567890") != std::string::npos)
			{
				cout << "您輸入了錯誤的帳戶數量 !";
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
							cout << "您輸入了錯誤的金額" << "\n";
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
				cout << "參數不足，無法執行" << "\n";
				exit(1);
			}
			if (!difficulty.empty())
			{
				cout << "請不要重新設定挖礦難度\n";
				exit(1);
			}
			difficulty = parameterStack.back();
			parameterStack.pop_back();
			if (!check_difficulty())
			{
				cout << "錯誤的挖礦難度值" << "\n";
				exit(1);
			}
		}
		else if (identifier == "-g")
		{
			if (check_parStackWoreOut())
			{
				cout << "參數不足，無法執行" << "\n";
				exit(1);
			}
			if (!gas.empty())
			{
				cout << "請不要重新設定交易手續費上限\n";
				exit(1);
			}
			gas = parameterStack.back();
			parameterStack.pop_back();
			if (!check_gasLimit())
			{
				cout << "錯誤的交易手續費值" << "\n";
				exit(1);
			}
		}
		else if (identifier == "-n")
		{
			if (check_parStackWoreOut())
			{
				cout << "參數不足，無法執行" << "\n";
				exit(1);
			}
			if (!nonce.empty())
			{
				cout << "請不要重新設定亂數\n";
				exit(1);
			}
			nonce = parameterStack.back();
			parameterStack.pop_back();
			if (!check_nonce())
			{
				cout << "錯誤的亂術數值" << "\n";
				exit(1);
			}
		}
		else if (identifier == "-p")
		{
			if (isPoA)
			{
				cout << "請不要重新指定 PoA 共識演算法\n";
				exit(1);
			}
			isPoA = true;
		}
		else if (identifier == "-w")
		{
			if (check_parStackWoreOut())
			{
				cout << "參數不足，無法執行" << "\n";
				exit(1);
			}
			if (!networkID.empty())
			{
				cout << "請不要重新設定 network ID.\n";
				exit(1);
			}
			networkID = parameterStack.back();
			parameterStack.pop_back();
			if (!check_networkID())
			{
				cout << "錯誤的 network ID." << "\n";
				exit(1);
			}
		}
		else
		{
			cout << "您指定了錯誤的參數" << "\n";
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
		cout << "您必須設定至少 2 個帳號來啟動 PoA 共識演算法\n";
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
	cout << "私有鏈設定快速檢視: \n";
	cout << "Chain ID : \t\t" << chainID << "\n";
	cout << "Network ID : \t\t" << networkID << "\n";
	cout << "亂數 : \t\t\t" << nonce << "\n";
	cout << "交易手續費上限 : \t" << gas << "\n";
	cout << "難度 : \t\t\t" << difficulty << "\n";
	cout << "共識演算法 : \t\tClique (PoA)" << "\n";
	cout << "資料儲存位置 : \t\t" << dataLoc;
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		cout << "/";
	}
	cout << "chain-" << chainID << "-P*" << "/\n";
	cout << "\n";
	cout << "預先設定的認證帳戶 \n";
	for (int i = 0; i < totalAccountCount; i++)
	{
		cout << "P" << i << ":\t" << accounts[i] << "\n";
		cout << "\t\t密碼:\t\t" << passwords[i] << "\n";
		cout << "\t\t預存金額:\t" << amounts[i] << "\n";
		cout << "\t\t埠號:\t\t" << to_string(30303 + i) << "\n";
	}

	cout << "\n";

	cout << "為了能夠方便您模擬多節點環境\n";
	cout << "認證帳戶已經分散到個別的資料儲存位置中\n";
	cout << "請執行每個資料儲存位置中的 init.sh 來初始化每個節點，執行一次即可 \n";
	cout << "之後請執行每個資料儲存位置中的 boot.sh 來啟動節點 \n";

	cout << "\n";

	cout << "為了避免潛在的資訊安全問題 \n";
	cout << "自動探尋功能預設已經關閉 \n";
	cout << "您可以移除 boot.sh 中的 --nodiscover 選項\n";
	cout << "或者手動加入各個節點\n";

	cout << "\n";

	cout << "執行時，請確定有 2 個或更多的認證節點上線 \n";

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
	cout << "私有鏈設定快速檢視: \n";
	cout << "Chain ID : \t\t" << chainID << "\n";
	cout << "Network ID : \t\t" << networkID << "\n";
	cout << "亂數 : \t\t\t" << nonce << "\n";
	cout << "交易手續費上限 : \t" << gas << "\n";
	cout << "難度 : \t\t\t" << difficulty << "\n";
	cout << "共識演算法 : \t\tetHash (PoW)" << "\n";
	cout << "資料儲存位置 : \t\t" << dataLoc;
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		cout << "/";
	}
	cout << "chain-" << chainID << "/\n";

	cout << "\n";
	if (totalAccountCount > 0)
	{
		cout << "預先設定的帳戶 \n";
		for (int i = 0; i < totalAccountCount; i++)
		{
			cout << i << ":\t" << accounts[i] << "\n";
			cout << "\t\t密碼:\t\t" << passwords[i] << "\n";
			cout << "\t\t預存金額:\t" << amounts[i] << "\n";
		}
	}
	else
	{
		cout << "您沒有預先設定帳戶 \n";
	}
	
	cout << "\n";	
	cout << "您必須先初始化您的私有鏈，才能使用。\n";
	cout << "執行一次 " << iSFilePath << " 初始化您的私有鏈 \n";
	cout << "往後執行 " << bSFilePath << " 來啟動您的私有鏈\n";
	
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
		cout << "您必須設定至少 2 個帳號來啟動 PoA 共識演算法";
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
			cout << "無法讀取 Ethereum 用戶端的輸出內容 \n";
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
			cout << "無法讀取 Ethereum 用戶端的輸出內容 \n";
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
		cout << "私有鏈 " << chainID << " 已經建立 \n";
		exit(1);
	}
	if (totalAccountCount < 2)
	{
		cout << "您必須設定至少 2 個帳號來啟動 PoA 共識演算法 \n";
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
		cout << "私有鏈 " << chainID << " 已經建立 \n";
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
		cout << "輸入您想要的 Chain ID \n";
		getline(cin, chainID);
	} while (!check_chainID());

	do
	{
		cout << "輸入您想要的 network ID，或輸入 X 自動設定 \n";
		getline(cin, networkID);
		if (networkID.compare("X") == 0)
		{
			auto_networkID();
		}
	} while (!check_networkID());

	do
	{
		cout << "輸入您想要的亂數值，或輸入 X 自動設定 \n";
		getline(cin, nonce);
		if (nonce.compare("X") == 0)
		{
			auto_nonce();
			cout << "自動設定亂數值為 " << nonce << "\n";
		}
	} while (!check_nonce());

	do
	{
		cout << "輸入您想要的交易手續費上限，或輸入 X 自動設定 \n";
		getline(cin, gas);
		if (gas.compare("X") == 0)
		{
			auto_gasLimit();
			cout << "自動設定交易手續費上限為 " << gas << "\n";
		}
		
	} while (!check_gasLimit());

	do
	{
		cout << "輸入您想要的挖礦難度，或輸入 X 自動設定 \n";
		getline(cin, difficulty);
		if (difficulty.compare("X") == 0)
		{
			cout << "請輸入您想要的挖礦難度 \n";
			cout << "\t0: 最低\n";
			cout << "\t1: 低\n";
			cout << "\t2: 中\n";
			cout << "\t3: 高\n";
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
		cout << "您想要建立帳戶嗎 (Y/N)? \n";
		getline(cin, accountInput);

	} while (accountInput.length() == 1 && accountInput.find_first_not_of("YNyn") != std::string::npos);

	if (accountInput == "Y" || accountInput == "y")
	{
		string accountAmount;
		do
		{
			cout << "您想建立多少帳戶 ? \n";
			getline(cin, accountAmount);
		} while (accountAmount.at(0) == '0' || accountAmount.find_first_not_of("0123456789") != std::string::npos);
		
		string accountPrealloc;
		string accountPreallocInput = "";

		do
		{
			cout << "您想要預先設定多少金額給每個帳戶 (Y/N)? \n";
			getline(cin, accountPrealloc);

		} while (accountPrealloc.length() == 1 && accountPrealloc.find_first_not_of("YyNn") != std::string::npos);

		if (accountPrealloc == "Y" || accountPrealloc == "y")
		{
			do
			{
				cout << "輸入您想要的金額（Wei）" << endl;
				getline(cin, accountPreallocInput);
			} while (accountPreallocInput.at(0) == '0' || accountPreallocInput.find_first_not_of("1234567890") != std::string::npos);

		}	

		string accountPassword;
		string accountPasswordInput = "";
		do
		{
			cout << "您想要為每個帳戶預設密碼嗎 (Y/N)? \n";
			getline(cin, accountPassword);
		} while (accountPassword.length() == 1 && accountPassword.find_first_not_of("YyNn") != std::string::npos);
		
		if (accountPassword == "Y" || accountPassword == "y")
		{
			do
			{
				cout << "請輸入密碼 \n";
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
		cout << "不支援的 Chain ID \n";
		return false;
	}
	else if (chainID.length() < 2)
	{
		cout << "為了避免衝突，請輸入 2 位數以上的 Chain ID \n";
		return false;
	}
	else if (chainID.at(0) == '0')
	{
		cout << "Chain ID 不應該以 0 起頭 \n";
		return false;
	}
	return chainID.find_first_not_of("0123456789") == std::string::npos;
}

bool check_networkID()
{
	if (networkID.length() > 5)
	{
		cout << "不支援的 Network ID \n";
		return false;
	}
	else if (networkID.length() < 2)
	{
		cout << "為了避免衝突，請輸入 2 位數以上的 network ID \n";
		return false;
	}
	else if (networkID.at(0) == '0')
	{
		cout << "network ID 不應該以 0 起頭 \n";
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
		cout << "挖礦難度不應該以 0 起頭 \n";
		return false;
	}
	if (difficulty.length() > 6)
	{
		cout << "請注意，設定高挖礦難度會導致效能下降 \n";
	}
	return difficulty.find_first_not_of("0123456789abcdef") == std::string::npos;
}

bool check_gasLimit()
{
	if (gas.at(0) == '0')
	{
		cout << "交易手續費上限不應該以 0 起頭 \n";
		return false;
	}
	if (gas.length() > 8)
	{
		cout << "您設定的交易手續費超過系統可以接受的範圍 \n";
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
			cout << "錯誤的設定檔案 \n";
			config_Reset();
		}
	}
	else
	{
		ifc.close();
		cout << "找不到設定檔案 \n";
		config_Reset();
	}
}

void config_Reset()
{
	cout << "請輸入 Ethereum 用戶端位置 ? \n";
	string toGethLoc;
	getline(cin, toGethLoc);
	gethLoc.assign(toGethLoc);
	cout << "請輸入私有鏈儲存位置 ? \n";
	string toDataLoc;
	getline(cin, toDataLoc);
	dataLoc.assign(toDataLoc);
	cout << "設定檔案已重設! \n";
	ofstream ofc;
	ofc.open("config.ini");
	ofc << "geth=" << toGethLoc << "\n";
	ofc << "data=" << toDataLoc << "\n";
	ofc.flush();
	ofc.close();
}

void auto_networkID()
{
	cout << "自動設定 chainID " << chainID << "為 network ID\n";
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
		cout << "最低挖礦難度 \n";
		cout << "請只用於實驗環境之中使用 \n";
		difficulty.assign("1");
	}
	else if (level > 0 && level <= 3)
	{
		string naming[] = {"低挖礦難度", "中挖礦難度", "高挖礦難度"};
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
		cout << "您輸入的挖礦難度無法辨識 \n";
	}
}

void auto_gasLimit()
{
	cout << "自動設定交易手續費上限為系統上限值 \n";
	gas.assign("ffffffff");
}

void auto_autoAccount(string password, string preAlloc)
{
	if (totalAccountCount < 1)
	{
		cout << "帳戶數量設定值錯誤 \n";
		exit(1);
	}
	else if (totalAccountCount > 100)
	{
		cout << "注意！建立過多的帳戶需要一點時間 \n";
	}
	if (password.empty())
	{
		cout << "您沒有指定預設密碼，自動設定每個帳號 \n";
		passwords = new string[totalAccountCount];
		for (int i = 0; i < totalAccountCount; i++)
		{
			string genPassword = auto_password(6);
			passwords[i] = genPassword;
		}

	}
	else
	{
		cout << "您指定了預設密碼 \n";
		passwords = new string[totalAccountCount];
		for (int i = 0; i < totalAccountCount; i++)
		{
			passwords[i] = password;
		}
	}

	if (preAlloc.empty())
	{
		cout << "您沒有設定每個帳號的預設金額 \n";
		cout << "設定為 0 Wei\n";
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
			cout << "設定金額不應以 0 起頭 \n";
			exit(1);
		}
		else if (preAlloc.find_first_not_of("0123456789") != std::string::npos)
		{
			cout << "錯誤的金額 \n";
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
