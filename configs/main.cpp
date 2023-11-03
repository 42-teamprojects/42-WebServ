#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;
string getdata(string filename);
vector<string> getinfo(const string &content, string tag);
void sto(string &text);
int main()
{
    string filename = "class.xml";
    string tag = "name";
    // string tag = "price";
    bool sto = true;
    string content = getFile(filename);
    vector<string> all = getData(content, tag);
    for (string &a : all)
    {
        if (sto)
            sto(a);
        cout << a << '\n';
    }
}
string getdata(string filename)
{
    string buff;
    char ch;
    ifstream in(filename);
    if (!in)
    {
        cout << filename << " not found";
        exit(1);
    }
    while (in.get(ch))
        buff += ch;
    in.close();
    return buff;
}
vector<string> getinfo(const string &content, string tag)
{
    vector<string> take;
    unsigned int pos = 0, st;
    while (true)
    {
        st = content.find("<" + tag, pos);
        if (st == string::npos)
            return take;
        st = content.find(">", start);
        st++;
        pos = content.find("</" + tag, st);
        if (pos == string::npos)
            return take;
        take.push_back(content.substr(st, pos - st));
    }
}
void sto(string &text)
{
    unsigned int sta = 0, ppp;
    while (sta < text.size())
    {
        sta = text.find("<", sta);
        if (sta == string::npos)
            break;
        ppp = text.find(">", sta);
        if (ppp == string::npos)
            break;
        text.erase(sta, ppp - sta + 1);
    }
}