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
#include <limits.h>
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

string getExePath();
string getExeLocation();

string programName = "";

int main(int argc, char **argv)
{
	string progNameLocal(argv[0]);
	if (progNameLocal.find("./") != std::string::npos)
	{
		progNameLocal = progNameLocal.substr(2, progNameLocal.length() -2);
	}
	programName = progNameLocal;

	config_Load();
	if (argc < 2)
	{
		cout << "\n";
		cout << "简易乙太坊私有链创建工具 第四版 / Linux平台" << "\n";
		cout << "操作方式 : createChain [-i/-c/-r] [chain ID] [其他选项]" << "\n";
		cout << "\t-i [其他选项...]\t\t 互动式创建" << "\n";
		cout << "\t-c [chain ID] [其他选项...]\t 创建私有链，必须配合其他选项" << "\n";
		cout << "\t-r [chain ID] [其他选项...]\t 移除私有链" << "\n";
		cout << "\n";
		cout << "[-i (互动式创建) 适用的其他选项] : " << "\n";
		cout << "\t-p \t\t\t <可选> \t 创建的私有链使用权威证明共识算法" << "\n";
		cout << "\n";
		cout << "[-c (创建) 适用的其他选项] : " << "\n";
		cout << "\t[chain ID] \t\t <必要> \t 私有链的 chain ID" << "\n";
		cout << "\n";
		cout << "\t-a [pw:am]... \t\t <可选> \t 创建账户" << "\n";
		cout << "\t\t pw\t\t <必要> \t 账户密码" << "\n";
		cout << "\t\t am\t\t <必要> \t 账户预存金额" << "\n";
		cout << "\t\t 范例 -a pass1:10000 pass2:20000" << "\n";
		cout << "\n";
		cout << "\t-b [num] [pw] [am] \t <可选> \t 账户批量创建工具" << "\n";
		cout << "\t\t num\t\t <必要> \t 创建账户数量" << "\n";
		cout << "\t\t pw\t\t <可选> \t 默认密码" << "\n";
		cout << "\t\t am\t\t <可选> \t 默认预存金额" << "\n";
		cout << "\t\t 范例 -b 3 pass 20000" << "\n";
		cout << "\n";
		cout << "\t-d [difficulty] \t <可选> \t 私有链初始挖矿难度" << "\n";
		cout << "\t\t difficulty \t <必要> \t 一组代表初始挖矿难度十进制数值" << "\n";
		cout << "\n";
		cout << "\t-g [gasLimit] \t\t <可选> \t 私有链手续费上限" << "\n";
		cout << "\t\t gasLimit \t <必要> \t 一组最多八位代表手续费上限的十六进制数值" << "\n";
		cout << "\n";
		cout << "\t-n [nonce] \t\t <可选> \t 私有链的杂凑值" << "\n";
		cout << "\t\t nonce \t\t <必要> \t 一组最多十六位代表杂凑值的十六进制数值" << "\n";
		cout << "\n";
		cout << "\t-p \t\t\t <可选> \t 创建的私有链使用权威证明共识算法" << "\n";
		cout << "\n";
		cout << "\t-w [networkID] \t\t <可选> \t 私有链的 network ID" << "\n";
		cout << "\t\t networkID \t <必要> \t 一组代表 network ID 的十进制数值" << "\n";
		cout << "\n";
		cout << "[-r (移除) 适用的其他选项] : " << "\n";
		cout << "\t[chain ID] \t\t <必要> \t 要移除的私有链的 chain ID" << "\n";
		cout << "\t-p \t\t\t <可选> \t 要移除的私有链使用权威证明共识算法" << "\n";
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
			cout << "缺省关键参数" << "\n";
			exit(1);
		}
		chainID = parameterStack.back();
		parameterStack.pop_back();

		if (!check_chainID)
		{
			cout << "您指定了非法的 chain ID" << "\n";
			exit(1);
		}

		ui_Unattended();
	}
	else if (identifier == "-r")
	{
		if (check_parStackWoreOut())
		{
			cout << "缺省关键参数\n";
			exit(1);
		}

		chainID = parameterStack.back();
		parameterStack.pop_back();

		if (!check_chainID)
		{
			cout << "您指定了非法的 chain ID" << "\n";
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
				cout << "私有链 " << chainID << " 不存在" << "\n";
				exit(1);
			}
			ui_RemovePoA();
		}
		else
		{
			if (!check_chainExist())
			{
				cout << "私有链 " << chainID << " 不存在" << "\n";
				exit(1);
			}
			ui_Remove();
		}

	}
	else
	{
		cout << "指定行为无法辨识\n";
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
				cout << "缺省关键参数" << "\n";
				exit(1);
			}

			if (totalAccountCount != 0)
			{
				cout << "请勿重复指定账户\n";
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
						cout << "账户格式不正确" << "\n";
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
				cout << "请勿重复指定账户\n";
				exit(1);
			}

			bool noAccPassword = false;
			bool noAccAmount = false;
			string accPassword = "";
			string accAmount = "";

			if (check_parStackWoreOut())
			{
				cout << "缺省关键参数" << "\n";
				exit(1);
			}
			string accNumberString = parameterStack.back();
			parameterStack.pop_back();
			if (accNumberString.at(0) == '0' || accNumberString.find_first_not_of("1234567890") != std::string::npos)
			{
				cout << "请输入正确的数量\n";
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
							cout << "请输入正确的金额数量" << "\n";
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
				cout << "缺省关键参数" << "\n";
				exit(1);
			}
			if (!difficulty.empty())
			{
				cout << "请勿重复指定难度\n";
				exit(1);
			}
			difficulty = parameterStack.back();
			parameterStack.pop_back();
			if (!check_difficulty())
			{
				cout << "不正确的难度" << "\n";
				exit(1);
			}
		}
		else if (identifier == "-g")
		{
			if (check_parStackWoreOut())
			{
				cout << "缺省关键参数" << "\n";
				exit(1);
			}
			if (!gas.empty())
			{
				cout << "请勿重复指定手续费上限\n";
				exit(1);
			}
			gas = parameterStack.back();
			parameterStack.pop_back();
			if (!check_gasLimit())
			{
				cout << "不正确的手续费上限" << "\n";
				exit(1);
			}
		}
		else if (identifier == "-n")
		{
			if (check_parStackWoreOut())
			{
				cout << "缺省关键参数" << "\n";
				exit(1);
			}
			if (!nonce.empty())
			{
				cout << "请勿重复指定杂凑\n";
				exit(1);
			}
			nonce = parameterStack.back();
			parameterStack.pop_back();
			if (!check_nonce())
			{
				cout << "不正确的杂凑" << "\n";
				exit(1);
			}
		}
		else if (identifier == "-p")
		{
			if (isPoA)
			{
				cout << "请勿重复指定权威证明共识算法\n";
				exit(1);
			}
			isPoA = true;
		}
		else if (identifier == "-w")
		{
			if (check_parStackWoreOut())
			{
				cout << "缺省关键参数" << "\n";
				exit(1);
			}
			if (!networkID.empty())
			{
				cout << "请勿重复指定 networkID.\n";
				exit(1);
			}
			networkID = parameterStack.back();
			parameterStack.pop_back();
			if (!check_networkID())
			{
				cout << "不正确的参数" << "\n";
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
		cout << "必须创建至少两个以上的帐号以启动权威证明共识算法\n";
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
	cout << "私有链设置快速检视 \n";
	cout << "Chain ID : \t\t" << chainID << "\n";
	cout << "Network ID : \t\t" << networkID << "\n";
	cout << "杂凑 : \t\t\t" << nonce << "\n";
	cout << "手续费上限 : \t\t" << gas << "\n";
	cout << "难度 : \t\t\t" << difficulty << "\n";
	cout << "共识算法 : \t\tClique (权威证明共识算法)" << "\n";
	cout << "数据库 : \t\t" << dataLoc;
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		cout << "/";
	}
	cout << "chain-" << chainID << "-P*" << "/\n";
	cout << "\n";
	cout << "预先设置的权威证明账户 \n";
	for (int i = 0; i < totalAccountCount; i++)
	{
		cout << "P" << i << ":\t" << accounts[i] << "\n";
		cout << "\t\t密码:\t\t" << passwords[i] << "\n";
		cout << "\t\t预存金额:\t" << amounts[i] << "\n";
		cout << "\t\t端口:\t\t" << to_string(30303 + i) << "\n";
	}

	cout << "\n";

	cout << "为了让您更加简便地模拟多节点环境\n";
	cout << "每个权威证明帐户皆已分散至各自的数据库\n";
	cout << "请在每个数据库中执行一次 init.sh 来初始化\n";
	cout << "之后请使用 boot.sh 启动\n";

	cout << "\n";

	cout << "为了避免可能的网络安全问题\n";
	cout << "自动搜索功能已经关闭\n";
	cout << "您可以修改 boot.sh 移除 --nodiscover 选项\n";
	cout << "或者手动加入节点\n";

	cout << "\n";

	cout << "请确保至少 2 个及以上的节点上线 \n";

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
	cout << "私有链设置快速检视 \n";
	cout << "Chain ID : \t\t" << chainID << "\n";
	cout << "Network ID : \t\t" << networkID << "\n";
	cout << "杂凑 : \t\t\t" << nonce << "\n";
	cout << "手续费上限 : \t\t" << gas << "\n";
	cout << "难度 : \t\t\t" << difficulty << "\n";
	cout << "共识算法 : \t\tetHash (工作量证明共识算法)" << "\n";
	cout << "数据库 : \t\t" << dataLoc;
	if (dataLoc.at(dataLoc.length() -1) != '/')
	{
		cout << "/";
	}
	cout << "chain-" << chainID << "/\n";

	cout << "\n";
	if (totalAccountCount > 0)
	{
		cout << "预先设置的账户 \n";
		for (int i = 0; i < totalAccountCount; i++)
		{
			cout << i << ":\t" << accounts[i] << "\n";
			cout << "\t\t密码:\t\t" << passwords[i] << "\n";
			cout << "\t\t预存金额:\t" << amounts[i] << "\n";
		}
	}
	else
	{
		cout << "没有预先设置的账户 \n";
	}
	
	cout << "\n";	
	cout << "使用私有链前，您必须先进行初始化\n";
	cout << "执行 " << iSFilePath << " 来进行初始化，仅一次\n";
	cout << "往后执行 " << bSFilePath << " 启动您的私有链\n";

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
	string outputDir = getExeLocation();
	if (totalAccountCount < 2)
	{
		cout << "必须创建至少两个以上的帐号以启动权威证明共识算法";
		exit(1);
	}
	accounts = new string[totalAccountCount];
	for (int i = 0; i < totalAccountCount; i++)
	{
		string passwordLoc = outputDir + "password" + chainID + ".txt";
		string resultLoc = outputDir + "result" + chainID + ".txt";
		ofstream ofc;
		ofc.open(passwordLoc.c_str());
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
		command.append(" -password ");
		command.append(passwordLoc);
		command.append(" >> ");
		command.append(resultLoc);
		system(command.c_str());

		ifstream ifc;
		ifc.open(resultLoc.c_str());
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
			string rmCmd = "rm -rf " + resultLoc;
			system(rmCmd.c_str());
		}
		else
		{
			ifc.close();
			string rmCmd = "rm -rf " + resultLoc;
			cout << "无法从乙太链客户端取得信息 \n";
			system(rmCmd.c_str());
		}
		string rmCmd = "rm -rf " + passwordLoc;
		system(passwordLoc.c_str());

	}

}

void ui_CreateAccount()
{
	string outputDir = getExeLocation();
	accounts = new string[totalAccountCount];
	for (int i = 0; i < totalAccountCount; i++)
	{
		ofstream ofc;
		string passwordLoc = outputDir + "password" + chainID + ".txt";
		string resultLoc = outputDir + "password" + chainID + ".txt";
		ofc.open(passwordLoc.c_str());
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
		ifc.open(resultLoc.c_str());
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
			string  rmCmd = "rm -rf " + resultLoc;
			system(rmCmd.c_str());
		}
		else
		{
			ifc.close();
			cout << "无法从乙太链客户端取得信息 \n";
			string rmCmd = "rm -rf " + resultLoc;
			system(rmCmd.c_str());
			exit(1);
		}
		string rmCmd = "rm -rf " + passwordLoc;
		system(rmCmd.c_str());

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
		cout << "必须创建至少两个以上的帐号以启动权威证明共识算法\n";
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
		cout << "私有链 " << chainID << " 已存在 \n";
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
		cout << "请输入想要的 chain ID \n";
		getline(cin, chainID);
	} while (!check_chainID());

	do
	{
		cout << "请输入想要的 network ID，或输入X进行自动设定 \n";
		getline(cin, networkID);
		if (networkID.compare("X") == 0)
		{
			auto_networkID();
		}
	} while (!check_networkID());

	do
	{
		cout << "请输入想要的杂凑，或输入X进行自动设定 \n";
		getline(cin, nonce);
		if (nonce.compare("X") == 0)
		{
			auto_nonce();
			cout << "杂凑自动设定为 " << nonce << "\n";
		}
	} while (!check_nonce());

	do
	{
		cout << "请输入想要的手续费上限，或输入X进行自动设定 \n";
		getline(cin, gas);
		if (gas.compare("X") == 0)
		{
			auto_gasLimit();
			cout << "手续费上限自动设定为 " << gas << "\n";
		}
	} while (!check_gasLimit());

	do
	{
		cout << "输入想要的难度，或输入X进行自动设定 \n";
		getline(cin, difficulty);
		if (difficulty.compare("X") == 0)
		{
			cout << "请决定想要的难度 \n";
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
		cout << "您想要设定账户吗 (Y/N)? \n";
		getline(cin, accountInput);

	} while (accountInput.length() == 1 && accountInput.find_first_not_of("YNyn") != std::string::npos);

	if (accountInput == "Y" || accountInput == "y")
	{
		string accountAmount;
		do
		{
			cout << "您想创建几个账户 ? \n";
			getline(cin, accountAmount);
		} while (accountAmount.at(0) == '0' || accountAmount.find_first_not_of("0123456789") != std::string::npos);
		string accountPrealloc;
		string accountPreallocInput = "";

		do
		{
			cout << "您想设定账户的默认预存金额吗 (Y/N)? \n";
			getline(cin, accountPrealloc);

		} while (accountPrealloc.length() == 1 && accountPrealloc.find_first_not_of("YyNn") != std::string::npos);

		if (accountPrealloc == "Y" || accountPrealloc == "y")
		{
			do
			{
				cout << "请输入预存金额" << endl;
				getline(cin, accountPreallocInput);
			} while (accountPreallocInput.at(0) == '0' || accountPreallocInput.find_first_not_of("1234567890") != std::string::npos);

		}

		string accountPassword;
		string accountPasswordInput = "";
		do
		{
			cout << "您想设定账户的默认密码吗 (Y/N)? \n";
			getline(cin, accountPassword);
		} while (accountPassword.length() == 1 && accountPassword.find_first_not_of("YyNn") != std::string::npos);

		if (accountPassword == "Y" || accountPassword == "y")
		{
			do
			{
				cout << "请输入密码 \n";
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
		cout << "非法的 Chain ID \n";
		return false;
	}
	else if (chainID.length() < 2)
	{
		cout << "避免磕碰，Chain ID 应大于 2 位数\n";
		return false;
	}
	else if (chainID.at(0) == '0')
	{
		cout << "Chain ID 不能以 0 起头 \n";
		return false;
	}
	return chainID.find_first_not_of("0123456789") == std::string::npos;
}

bool check_networkID()
{
	if (networkID.length() > 5)
	{
		cout << "非法的 Network ID \n";
		return false;
	}
	else if (networkID.length() < 2)
	{
		cout << "避免磕碰，Network ID 应大于 2 位数 \n";
		return false;
	}
	else if (networkID.at(0) == '0')
	{
		cout << "networkID 不能以 0 起头 \n";
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
		cout << "难度不能以 0 起头 \n";
		return false;
	}
	if (difficulty.length() > 6)
	{
		cout << "过高的难度会导致效率低落 \n";
	}
	return difficulty.find_first_not_of("0123456789abcdef") == std::string::npos;
}

bool check_gasLimit()
{
	if (gas.at(0) == '0')
	{
		cout << "交易手续费上限不能以 0 起头 \n";
		return false;
	}
	if (gas.length() > 8)
	{
		cout << "您设置的交易手续费上限已超过系统允许范围 \n";
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
	string outputDir = getExeLocation();
	string configLoc = outputDir + "config.ini";
	ifstream ifc;
	ifc.open(configLoc.c_str());
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
			cout << "非法的设置文件 \n";
			config_Reset();
		}
	}
	else
	{
		ifc.close();
		cout << "找不到设置文件 \n";
		config_Reset();
	}
}

void config_Reset()
{
	string outputDir = getExeLocation();
	cout << "请输入乙太坊客户端位置 ? \n";
	string toGethLoc;
	getline(cin, toGethLoc);
	gethLoc.assign(toGethLoc);
	cout << "请输入数据库存储位置 ? \n";
	string toDataLoc;
	getline(cin, toDataLoc);
	dataLoc.assign(toDataLoc);
	cout << "设置文件已重置 \n";
	ofstream ofc;
	string configLoc = outputDir + "config.ini";
	ofc.open(configLoc.c_str());
	ofc << "geth=" << toGethLoc << "\n";
	ofc << "data=" << toDataLoc << "\n";
	ofc.flush();
	ofc.close();
}

void auto_networkID()
{
	cout << "自动设置 chainID " << chainID << " 为 networkID\n";
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
		cout << "最低挖矿难度 \n";
		cout << "请用于实验环境 \n";
		difficulty.assign("1");
	}
	else if (level > 0 && level <= 3)
	{
		string naming[] = {"低挖矿难度", "中挖矿难度", "高挖矿难度"};
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
		cout << "指定挖矿难度不存在 \n";
	}
}

void auto_gasLimit()
{
	cout << "自动设置交易手续费上限为系统允许最大值 \n";
	gas.assign("ffffffff");
}

void auto_autoAccount(string password, string preAlloc)
{
	if (totalAccountCount < 1)
	{
		cout << "自动创建并设置帐号出现问题 \n";
		exit(1);
	}
	else if (totalAccountCount > 100)
	{
		cout << "创建大量帐号将花费一点时间 \n";
	}
	if (password.empty())
	{
		cout << "您没有指定默认密码，自动设置 \n";
		passwords = new string[totalAccountCount];
		for (int i = 0; i < totalAccountCount; i++)
		{
			string genPassword = auto_password(6);
			passwords[i] = genPassword;
		}

	}
	else
	{
		cout << "您指定了默认密码 \n";
		passwords = new string[totalAccountCount];
		for (int i = 0; i < totalAccountCount; i++)
		{
			passwords[i] = password;
		}
	}

	if (preAlloc.empty())
	{
		cout << "没有指定默认预存金额 \n";
		cout << "设置为 0 Wei\n";
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
			cout << "预存金额不能以 0 起头 \n";
			exit(1);
		}
		else if (preAlloc.find_first_not_of("0123456789") != std::string::npos)
		{
			cout << "您指定了非法的预存金额 \n";
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

string getExeLocation()
{
        int programNameSize = programName.length();
        return (getExePath()).substr(0, (getExePath()).length() - programNameSize);
}

string getExePath()
{
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        return std::string(result, (count > 0) ? count : 0);
}
