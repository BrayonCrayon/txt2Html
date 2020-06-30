//Braydon Deroy
//March 9, 2015
//This program will convert an ASCII text file into a xhtml 1.0 text file.

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <list>
#include <regex>
#include <locale>


using namespace std;

int main(int argc, char* argv[])
{
	//Thousands seperator
	struct seperator : numpunct < char >
	{
	protected:
		virtual char do_thousands_sep() const override { return ','; }
		virtual string do_grouping() const override { return "\03"; }
	};
	cout.imbue(locale(cout.getloc(), new seperator));

	//Checks to see if there are the right amount of argumants
	if (argc < 2 || argc > 4)
	{
		cout << "Error: incorrect number of comand line argumants\n"
			"Usage: txt2html filename" << endl;
		return EXIT_FAILURE;
	}

	bool rFound = false;
	string rSwitch = "-r";
	string test = "";
	//finding the -r for the report
	for (size_t i = 1; i < argc; ++i)
	{
		test += (string)argv[i];
		regex rTest(test, regex::icase);
		if (regex_match(rSwitch, rTest))
		{
			rFound = true;
			
		}
		if (rFound)
			break;
		
		test.erase(test.begin(), test.end());
	}

	//finding an .html extension if any are visible
	string outFileName = "";
	bool htmlFound = false;
	test.erase(test.begin(), test.end());
	string exTest = "";
	for (size_t i = 1; i < argc; ++i)
	{
		test += (string)argv[i];
		string exten = ".html";
		int testCounter = 0;

		//get the argument that matchs the extension
		for (int k = test.length() - 1; testCounter < 5; --k)
		{
			if (test.length() < 5)
				break;
			char ch = test.at(k);
			exTest += ch;
			++testCounter;
		}

		//reverseing the exTest varible
		reverse(exTest.begin(), exTest.end());
		
		//Comparing the file extension to see if it matches the .html extension
		regex hTest(exTest, regex::icase);
		if (regex_match(exten, hTest))
			htmlFound = true;

		if (htmlFound)
		{
			outFileName = test;
			break;
		}
		//erase the temperatory strings 
		test = "";
		exTest = "";
	}

	//finding the .txt file in the arguments
	test.erase(test.begin(), test.end());
	exTest.erase(exTest.begin(), exTest.end());
	string inPutFile = "";
	bool txtFound = false;
	for (size_t i = 1; i < argc; ++i)
	{
		test += (string)argv[i];
		string exten = ".txt";
		int testCounter = 0;
		//get the argument extension
		for (int k = test.length() - 1; testCounter < 4; --k)
		{
			if (test.length() < 4)
				break;
			char ch = test.at(k);
			exTest += ch;
			++testCounter;
		}
		
		
		//reversing the exTest varible
		reverse(exTest.begin(), exTest.end());

		//comparing the file extension to see if it matches the .txt extension
		regex txtTest(exten, regex::icase);
		if (regex_match(exTest, txtTest))
			txtFound = true;

		if (txtFound)
		{
			inPutFile = test;
			break;
		}
		test = "";
		exTest = "";
	}

	//if not html file specified 
	//then it will cut off the extension and add the html file extension.
	if (htmlFound == false && txtFound == true)
	{
		outFileName = inPutFile;
		int removeCh = 0;
		for (int i = outFileName.length() - 1; removeCh < 4; --i)
		{
			outFileName.erase(i);
			++removeCh;
		}
		string filePathCheck = "";
		for (int i = outFileName.length() - 1; i != -1; --i)
		{
			if (outFileName.at(i) == '/' || outFileName.at(i) == '\\' )
				break;
			filePathCheck += outFileName.at(i);
		}
		reverse(filePathCheck.begin(), filePathCheck.end());
		outFileName = filePathCheck;
		outFileName += ".html";
	}

	
	ifstream infile(inPutFile);
	ofstream htmlFile(outFileName);
	//Checks to see if the file exists, or if path is invalid
	if (!infile)
	{
		cout << "Error: failed to open " << inPutFile << endl
			<< "Check filename, path, or it doesn't exist.\n";
		return EXIT_FAILURE;
	}

	//Create a vector of typedef and fill it will the characters of the file
	char ch = 0;
	typedef list<char> dictionary_type;
		dictionary_type output;

	//hard code the begining
	string html = "<html xminx=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\n<head>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n";
	
	//Putting the input file into a vector of typedef
	int endOfFile = 0;
	while (infile.get(ch))
	{
		output.push_back(ch);
		if (ch == '\n')
			++endOfFile;
	}
	
	//applying the title into the outputfile
	htmlFile << html;
	htmlFile << "<title>";
	string title = "";
	for (int i = 0; i < inPutFile.length() - 4; ++i)
		title += inPutFile.at(i);
	//checking to see if there is any pathways for the title
	string filePathCheck = "";
	for (int i = title.length() - 1; i != -1; --i)
	{
		if (title.at(i) == '/' || title.at(i) == '\\' )
			break;
		filePathCheck += title.at(i);
	}
	reverse(filePathCheck.begin(), filePathCheck.end());
	title = filePathCheck;
	htmlFile << title;
	htmlFile << "</title>\n</head>\n";
	htmlFile << "<body>";

	//allpying break tags into the file
	int brTagCount = 0;
	string breakPoint = "<br />";
	list<char>::const_iterator it = output.begin();
	int inputLines = 0;
	int outputPara = 0;
	if (rFound == true)		//applying break points with a report.
	{		//applying break tags
		for (it; it != output.end(); ++it)
		{
			if (*it == '\n')
			{
				++inputLines;
				++brTagCount;
				if (inputLines == endOfFile)
				{
					for (int i = 0; i < brTagCount - 1; ++i)
					{
						++outputPara;
						htmlFile << breakPoint << "\n";
					}
				}
			}
			else
			{
				for (int i = 0; i < brTagCount - 1; ++i)
				{
					++outputPara;
					htmlFile << breakPoint << "\n";
				}
				brTagCount = 0;
			}
			if (brTagCount < 2)
				htmlFile << *it;
		}

	}
	else	//Applying break tags without a report
	{
		//applying the break tags
		for (it; it != output.end(); ++it)
		{
			if (*it == '\n')
			{
				++brTagCount;
				if (inputLines == endOfFile)
				{
					for (int i = 0; i < brTagCount - 1; ++i)
					{
						++outputPara;
						htmlFile << breakPoint << "\n";
					}
				}
			}
			else
			{
				for (int i = 0; i < brTagCount - 1; ++i)
					htmlFile << breakPoint << "\n";
				brTagCount = 0;
			}
			if (brTagCount < 2)
				htmlFile << *it;
		}
	}
	//adding the end of html file 
	htmlFile << "</body>\n</html>";
	//if report is found
	if (rFound)
	{
		cout << "Input lines processed: " << inputLines << endl;
		cout << "Output paragraphs processed: " << outputPara << endl;
	}
}
