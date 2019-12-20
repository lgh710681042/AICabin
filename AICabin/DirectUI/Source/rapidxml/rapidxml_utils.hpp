#ifndef RAPIDXML_UTILS_HPP_INCLUDED
#define RAPIDXML_UTILS_HPP_INCLUDED

// Copyright (C) 2006, 2009 Marcin Kalicinski
// Version 1.13
// Revision $DateTime: 2009/05/13 01:46:17 $
//! \file rapidxml_utils.hpp This file contains high-level rapidxml utilities that can be useful
//! in certain simple scenarios. They should probably not be used if maximizing performance is the main objective.

#include "rapidxml.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

namespace rapidxml
{

    //! Represents data loaded from a file
    template<class Ch = char>
    class file
    {
        
    public:
        
        //! Loads file into the memory. Data will be automatically destroyed by the destructor.
        //! \param filename Filename to load.
        file()
        {
        }
		bool open(LPCTSTR filename)
		{
			using namespace std;
			// Open stream
			basic_ifstream<Ch> stream(filename, ios::binary);
			if (!stream)
				return false;
			stream.unsetf(ios::skipws);

			// Determine stream size
			stream.seekg(0, ios::end);
			size_t size = stream.tellg();
			stream.seekg(0);   

			if (size == 0) return false;
			// Load data and add terminating 0
			m_data.resize(size + 1);
			stream.read(&m_data.front(), static_cast<streamsize>(size));
			m_data[size] = 0;
			return true;
		}

		bool parse(xml_document<Ch> & document)
		{
			try
			{
				const Ch * pdata = data();
				if (!pdata) return false;
				document.parse<0>(data());
			}
			catch(...)
			{
				return false;
			}
			return true;
		}

		bool open(Ch* ptr, size_t n)
		{
			if (!ptr || n == 0) return false;
			m_data.resize(n + 1);
			memcpy(&m_data.front(), ptr, n*sizeof(Ch));
			m_data[n] = 0;
			return true;
		}
        
        //! Gets file data.
        //! \return Pointer to data of file.
        Ch *data()
        {
            return &m_data.front();
        }

        //! Gets file data.
        //! \return Pointer to data of file.
        const Ch *data() const
        {
            return &m_data.front();
        }

        //! Gets file data size.
        //! \return Size of file data, in characters.
        std::size_t size() const
        {
            return m_data.size();
        }

    private:
        std::vector<Ch> m_data;   // File data
    };
}

#endif
