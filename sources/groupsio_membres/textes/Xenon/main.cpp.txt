// AdjacentWords utility
// Written by Chris 'Xenon' Hanson, xenon@arcticus.com
// Nov 21, 2010
// for the Kryptos solving project group
// all new code is fully Public domain
// all code included from other external examples is cited

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <locale>
#include <algorithm>

typedef std::map<std::string, unsigned long int> CountMap;
CountMap preceding, following;


// From http://gpwiki.org/forums/viewtopic.php?t=5774
// since I didn't want to require boost::tokenizer
std::vector<std::string> explode(std::string const& str, std::string const& delim) 
{ 
   using std::string; 
   string::size_type start(0); 
   string::size_type end(string::npos); 
   std::vector<string> sub; 

   while( (end = str.find(delim,start)) != string::npos) 
   { 
      sub.push_back(str.substr(start, end - start)); 
      start = end + delim.size();

	  // hack: handle multiple delimiters in a row (only works for single-char delimiters)
	  while(start != str.length() && str[start] == delim[0]) start++;
   } 
   if(start < str.length() )sub.push_back(str.substr(start, str.size() - start)); 
   return sub; 
} // explode


// from http://stackoverflow.com/questions/735204/convert-a-string-in-c-to-upper-case
// again to avoid Boost dependency
typedef std::string::value_type char_t;

char_t up_char( char_t ch )
{
    return std::use_facet< std::ctype< char_t > >( std::locale() ).toupper( ch );
}

std::string toupper( const std::string &src )
{
    std::string result;
    std::transform( src.begin(), src.end(), std::back_inserter( result ), up_char );
    return result;
}


// reverse-order comparison for largest-to-smallest sorting in sorted maps
// from http://www.sgi.com/tech/stl/Multimap.html
struct gtulint
{
  bool operator()(unsigned long int ulint1, unsigned long int ulint2) const
  {
    return ulint1 > ulint2;
  } // operator()
}; // gtulint

typedef std::multimap<unsigned long int, std::string, gtulint> SortedMap;



// Usage: AdjacentWords <targetword>
int main(int Count, char *Vector[])
{
	if(Count < 2) return(0);

	std::string targetArg = toupper(Vector[1]);

	std::string targetWord;

	targetWord = "< " + targetArg + " >"; // find the one marked by the concordance output, as multiple hits may be in one line

	unsigned long int numFinds = 0;
	for(bool keepGoing = true; keepGoing;)
	{
		// read a line
		std::string inLine;
		std::getline(std::cin, inLine);

		// make it all uppercase
		inLine = toupper(inLine);

		// search for target word
		size_t locationFound;
		if((locationFound = inLine.find(targetWord)) != inLine.npos)
		{
			numFinds++;

			// turn all non-letter glyphs into spaces
			std::locale loc;
			for (std::string::iterator it=inLine.begin(); it!=inLine.end(); ++it)
			{
				if (std::isalpha(*it,loc))
				{
				} // if
				else
				{
					*it = ' ';
				} // else
			} // for

			std::string targetMarker = "<<" + targetArg + ">>";
			// restore <> markings around main hit, omitting spaces that would break tokenizing below
			inLine.replace(locationFound, targetMarker.length(), targetMarker);

			// break into array of words
			std::vector<std::string> lineWords = explode(inLine, " ");

			if(lineWords.size() > 0)
			{
				// locate which entry is "<<TARGET>>"
				std::vector<std::string>::iterator targetIdx = std::find(lineWords.begin(), lineWords.end(), targetMarker);
				if(targetIdx != lineWords.end()) // was it found?
				{
					// locate preceding word
					std::string prefix;
					if(targetIdx != lineWords.begin()) // it target not the first word?
					{
						prefix = *(targetIdx - 1); // grab word from entry prior to iterator
						CountMap::iterator mapEntry;
						if((mapEntry = preceding.find(prefix)) != preceding.end())
						{
							// increment existing count
							mapEntry->second ++;
						} // if
						else
						{
							preceding[prefix] = 1;
						} // else
						
					} // if

					// locate following word
					std::string postfix;
					if(targetIdx + 1 != lineWords.end()) // it target not the last word?
					{
						postfix = *(targetIdx + 1); // grab word from entry after iterator
						CountMap::iterator mapEntry;
						if((mapEntry = following.find(postfix)) != following.end())
						{
							// increment existing count
							mapEntry->second ++;
						} // if
						else
						{
							following[postfix] = 1;
						} // else
					} // if

				} // if
			} // if
		} // if
		if(std::cin.eof()) keepGoing = false;

	} // for

	// print output
	std::cout << "Prefixes:" << std::endl;
	SortedMap sortedMapPrefix;
	// copy from sorted-by-word map to sorted-by-count multimap for output
	for(CountMap::iterator loop = preceding.begin(); loop != preceding.end(); loop++)
	{
		sortedMapPrefix.insert(std::pair<unsigned long int, std::string>(loop->second, loop->first));
	} // for
	for(SortedMap::iterator sortedLoop = sortedMapPrefix.begin(); sortedLoop != sortedMapPrefix.end(); sortedLoop++)
	{
		std::cout << sortedLoop->second << " " << targetArg << " : " << sortedLoop->first << std::endl;
	} // for

	std::cout << "----------" << std::endl << std::endl;

	std::cout << "Postfixes:" << std::endl;
	SortedMap sortedMapPostfix;
	// copy from sorted-by-word map to sorted-by-count multimap for output
	for(CountMap::iterator loop = following.begin(); loop != following.end(); loop++)
	{
		sortedMapPostfix.insert(std::pair<unsigned long int, std::string>(loop->second, loop->first));
	} // for
	for(SortedMap::iterator sortedLoop = sortedMapPostfix.begin(); sortedLoop != sortedMapPostfix.end(); sortedLoop++)
	{
		std::cout << targetArg << " " << sortedLoop->second << " : " << sortedLoop->first << std::endl;
	} // for

	std::cout << "----------" << std::endl << std::endl;
	std::cout << "Hit lines: " << numFinds;

	return(0);
} // main



