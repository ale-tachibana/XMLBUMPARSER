/*
MIT License

Copyright (c) 2017 Alessandro K. Tachibana

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <regex>
//#include <thread>
//#include <mutex>

#ifndef XMLBUM_INLINE
#define XMLBUM_INLINE inline
#endif

namespace xmlbum
{
	class xml_item : public std::enable_shared_from_this<xml_item>
	{
	protected:				
		enum bumconsts {
			kSearchDepth = 20
			//,kThreadLimit = 8
		};

		//static int threadcounter;
		
		std::string xml_value, xml_tag;
		
		std::shared_ptr<xml_item> xml_parent;
		std::shared_ptr<xml_item> xml_root;
		std::vector<std::shared_ptr<xml_item>> xml_searchitems;
		std::vector<std::shared_ptr<xml_item>> xml_subitems;
		std::unique_ptr<std::multimap<std::string, int>> xml_tag_index;
		std::map<std::string, std::string> xml_attributes;
		std::map<std::string, std::string> xml_declaration;
		
	public:		
		XMLBUM_INLINE
		std::shared_ptr<xml_item> parent();
		XMLBUM_INLINE
		std::shared_ptr<xml_item> root();
		XMLBUM_INLINE
		std::string value();
		XMLBUM_INLINE
		void value(std::string value);
		XMLBUM_INLINE
		std::string tag();
		XMLBUM_INLINE
		std::vector<std::shared_ptr<xml_item>> items();
		

		xml_item();
		xml_item(const std::string tag, const std::string value, std::shared_ptr<xml_item> xml_parent, std::shared_ptr<xml_item> xml_root);
		xml_item(const xml_item& other);
		//~xml_item();
				
		std::shared_ptr<xml_item> Item(const std::string tag);		
		std::string SearchValueOfItem(const std::string tag);
		//std::string SearchValueOfItem(std::string tag, int index);
			
		std::vector<std::shared_ptr<xml_item>> GetItems(const std::string tag);
		

		
		std::shared_ptr<xml_item> GetItem(const std::string tag);
		
		std::shared_ptr<xml_item> GetItem(const std::string tag, const int index);
		

		/*		
		XMLBUM_INLINE
		std::vector<std::shared_ptr<xml_item>> SearchItems(std::string tag, const int depth = kSearchDepth);		
		XMLBUM_INLINE
		std::vector<std::shared_ptr<xml_item>> SearchItems(std::string regexstr, const int depth  = kSearchDepth);
		*/		
		std::vector<std::shared_ptr<xml_item>> SearchItems(const std::string exacttag, const std::string regexstr);
		//return previously searched items;
		XMLBUM_INLINE
		std::vector<std::shared_ptr<xml_item>> searchitems();


		std::shared_ptr<xml_item> Add(const std::string tag, const std::string value);			
		void SetAttributes(const std::map<std::string, std::string> attributes);
		std::string GetAttribute(const std::string attrname);
		template <typename T> T GetAttributeT(const std::string attrname);	


		void SplitString(std::string input, std::vector<std::string> & out_string, char delimiter);
	};

	
	class xml_parser
	{
	protected:
		std::string trim(std::string str_in);
		void parse_attrib(std::string strattr, std::string &tag, std::map<std::string, std::string> &xml_attributes);
		std::shared_ptr<xml_item> parse_istream(std::istream * objFile);
	public:		
		xml_parser();
		std::shared_ptr<xml_item> parse_file(std::string filepath) {
			std::ifstream  objFile(filepath);

			return parse_istream((std::istream*) &objFile);
		};
		std::shared_ptr<xml_item> parse_string(std::string strinput) {
			std::istringstream  objFile(strinput);

			return parse_istream((std::istream*) &objFile);
		};

	};
	
}