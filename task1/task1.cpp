#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <iterator>
#include <CkSFtp.h>
#include <CkSFtpDir.h>
#include <CkSFtpFile.h>
//#include <mysql.h>

using namespace std;

string key(string line) {    
    int l = line.find_first_of('=');
    char k[256];
    k[line.copy(k, l, 0)] = '\0';
    return k;
}

string value(string line) {
    int l = line.find_first_of('=');
    char k[256];
    k[line.copy(k, line.size(), l+1)] = '\0';
    return k;
}

int main()
{
    //MYSQL* conn;

    string path;
    //path = "cfg.ini";
    cout << "Config file path: ";
    cin >> path;
    ifstream fin(path);
    if (!fin.is_open()) {
        cout << "Not found\n";
        return 0;
    }
    int sftp_port = 0;
    map<string, string> npd;
    for (string line;  getline(fin, line);) {
        npd.insert(make_pair(key(line), value(line)));
        cout << npd.find(key(line))->first << "=" << npd.find(key(line))->second << endl;
    }
    stringstream sti(npd.find("sftp_port")->second);
    sti >> sftp_port;
    CkSFtp sftp;
    char sftp_host[256], sftp_user[256], sftp_password[256], remoteDir[256], localDir[256];
    sftp_host[npd.find("sftp_host")->second.copy(sftp_host, npd.find("sftp_host")->second.size(), 0)] = '\0';
    sftp_user[npd.find("sftp_user")->second.copy(sftp_user, npd.find("sftp_user")->second.size(), 0)] = '\0';
    sftp_password[npd.find("sftp_password")->second.copy(sftp_password, npd.find("sftp_password")->second.size(), 0)] = '\0';
    remoteDir[npd.find("sftp_remote_dir")->second.copy(remoteDir, npd.find("sftp_remote_dir")->second.size(), 0)] = '\0';
    localDir[npd.find("local_dir")->second.copy(localDir, npd.find("local_dir")->second.size(), 0)] = '\0';
    if (!sftp.Connect(sftp_host, sftp_port)) {
        cout << "SFTP: Failed to establish TCP/IP connection" << endl;
        return 0;
    }
    else {
        cout << "SFTP: TCP/IP connection established." << endl;
    }

    if (!sftp.AuthenticatePw(sftp_user, sftp_password)) {
        cout << "SFTP: Authentication failed." << endl;
        return 0;
    }
    else {
        cout << "SFTP: Authentication successful." << endl;
    }
    if (!sftp.InitializeSftp()) {
        cout << "SFTP: Initialization failed." << endl;
        return 0;
    }
    else {
        cout << "SFTP: Initialization successful." << endl;
    }
    //cout << remoteDir << " >> " << localDir << endl;
    if (!sftp.SyncTreeDownload(remoteDir, localDir, 0, 0)) {
        cout << "SFTP: Invalid directory." << endl;
    }
    const char* handle = sftp.openDir(remoteDir);
    CkSFtpDir *list = sftp.ReadDir(handle);
    int n = list->get_NumFilesAndDirs();
    for (int i = 0; i < n; i++) {
        CkSFtpFile* obf = list->GetFileObject(i);
        const char* file = obf->filename();
        cout << file << " copied." << endl;
    }
    //sftp.SyncTreeDownload("/", "d:/fromsftp", 0, 0);
    //cout << sftp.lastErrorText() << endl;
    return 0;
}