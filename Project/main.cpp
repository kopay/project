#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<set>
#include<algorithm>
#include<vector>


using namespace std;


//Storing the grade records for processing
struct grade_entry {
	string firstName, lastName;
	int score,id; 
};

//Comparator of the Gradeentry (for the set container) - All the sorting stuff are inside this comparator
struct compararator_grade_entry {
	bool operator()(const grade_entry& right, const grade_entry& left) const
	{
		if (right.score > left.score)
			return true;
		else if (right.score == left.score)
		{
			//If scores are equal, then last names are checked
			if (right.lastName < left.lastName)
				return true;
			else if (right.lastName == left.lastName)
			{
				//If last name is also same, then first names are checked
				if (right.firstName < left.firstName)
					return true;
				else if (right.firstName == left.firstName)
				{
					//If everything same, compare the entry ids
					if (right.id < left.id)
						return true;
					else
						return false;
				}
				else
					return false;
			}
			else
				return false;

		}
		else
			return false;
	}
};

enum ERROR_CODE {SUCCESS, INVALID_SCORE};


ERROR_CODE sortGradeScores(string inputFileName)
{
	cout << "grade-scores " << inputFileName << endl;

	//Storing the grade entries inside an STL set container for automated sorting
	set<grade_entry, compararator_grade_entry> entries;

	//Processing the input text file
	ifstream inFile;
	
	inFile.open(inputFileName.c_str());

	int entry_id = 1;

	while (!inFile.eof())
	{
		string line;
		getline(inFile,line);

		stringstream ssLine(line);

		string word1, word2, word3;

		getline(ssLine, word1, ',');
		getline(ssLine, word2, ',');
		getline(ssLine, word3, ',');

		word1.erase(remove_if(word1.begin(), word1.end(), isspace), word1.end());
		word2.erase(remove_if(word2.begin(), word2.end(), isspace), word2.end());
		word3.erase(remove_if(word3.begin(), word3.end(), isspace), word3.end());

		grade_entry entry;
		entry.lastName = word1;
		entry.firstName = word2;
		entry.score = atoi(word3.c_str());
		entry.id = entry_id;

		if (entry.score == 0 && word3 != "0")
			return INVALID_SCORE;

		entries.insert(entry);
		entry_id++;
	}

	inFile.close();

	ofstream outFile;

	//Creating the output file name according to the format specified
	int ext_pos = inputFileName.find_last_of(".");
	
	int file_pos = inputFileName.find_last_of("\\");

	string outputFileName = inputFileName.substr(file_pos + 1, ext_pos - file_pos - 1) +"-graded.txt";

	//print the sorted entries and writing them to the relevant output file

	outFile.open(outputFileName.c_str());

	set<grade_entry, compararator_grade_entry>::iterator last_element = entries.end();
	last_element--;

	for (set<grade_entry, compararator_grade_entry>::iterator it = entries.begin(); it != entries.end(); it++)
	{
		cout << it->lastName << ", " << it->firstName << ", " << it->score << endl;
		outFile << it->lastName << ", " << it->firstName << ", " << it->score;

		if (it != last_element)
			outFile << endl;
	}

	outFile.close();

	cout << "Finished: created " << outputFileName << endl;

	//Success
	return SUCCESS;
}

/*********************************************************************************************/
//Unit Tests Implementations to test the algorithms

//Comparing two .txt files
bool compareTwoTextFiles(string fileNameR, string fileNameL)
{
	ifstream rFile, lFile;

	rFile.open(fileNameR);
	lFile.open(fileNameL);

	while ((!rFile.eof()) && (!lFile.eof()))
	{
		string rLine, lLine;
		getline(rFile, rLine);
		getline(lFile, lLine);

		if (rLine != lLine)
			return false;
	}

	if (rFile.eof() && lFile.eof())
		return true;
	else
		return false;
}


int main()
{
	//Test Cases are deemed successful based on the comparison between the output text file(file name ends with -graded.txt )
	//and the ground truth text file (created by the author and the file name ends with -test.txt)

	//Given Test Case
	sortGradeScores("names.txt");
	//_ASSERT(compareTwoTextFiles("names-graded.txt", "names-test.txt"));
	if (compareTwoTextFiles("names-graded.txt", "names-test.txt"))
		cout << "Given Test Success Full" << endl;

	//Test case 1
	//Different Marks, Different First Names and Differen Last Names
	sortGradeScores("1-names.txt");
	//_ASSERT(compareTwoTextFiles("1-names-graded.txt", "1-names-test.txt"));
	if(compareTwoTextFiles("1-names-graded.txt", "1-names-test.txt"))
		cout << "Test 1 Success Full" << endl;

	//Test case 2
	//Same Marks, Different First Names and Differen Last Names
	sortGradeScores("2-names.txt");
	//_ASSERT(compareTwoTextFiles("2-names-graded.txt", "2-names-test.txt"));
	if (compareTwoTextFiles("2-names-graded.txt", "2-names-test.txt"))
		cout << "Test 2 Success Full" << endl;

	//Test case 3
	//Same Marks Different First Names and Same Last Names
	sortGradeScores("3-names.txt");
	//_ASSERT(compareTwoTextFiles("3-names-graded.txt", "3-names-test.txt"));
	if (compareTwoTextFiles("3-names-graded.txt", "3-names-test.txt"))
		cout << "Test 3 Success Full" << endl;

	//Test case 4
	//Same Marks Same First Names Different Last Names
	sortGradeScores("4-names.txt");
	//_ASSERT(compareTwoTextFiles("4-names-graded.txt", "4-names-test.txt"));
	if (compareTwoTextFiles("4-names-graded.txt", "4-names-test.txt"))
		cout << "Test 4 Success Full" << endl;

	//Test case 5
	//Same Marks Same First Names Same Last Names
	sortGradeScores("5-names.txt");
	//_ASSERT(compareTwoTextFiles("5-names-graded.txt", "5-names-test.txt"));
	if (compareTwoTextFiles("5-names-graded.txt", "5-names-test.txt"))
		cout << "Test 5 Success Full" << endl;

	//Test case 6
	//Checking for Invalid Scores
	sortGradeScores("6-names.txt");
	//_ASSERT(sortGradeScores("6-names.txt") == INVALID_SCORE);
	if (sortGradeScores("6-names.txt") == INVALID_SCORE)
		cout << "Test 6 Success Full" << endl;

}

