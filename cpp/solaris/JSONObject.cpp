/**
 * @file    JSONObject.hpp
 * @class   JSONObject
 * @author  Mario Antunes (mariolpantunes@gmail.com)
 * @version 1.0
 * @date    January, 2013
 *
 * JSONObject is a easy to use JSON wrapper for boost property tree.
 * Based on the API of Java org.JSON library (http://www.json.org/java/index.html).
 */

#include "JSONObject.hpp"

#include <iostream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#define NUMBER "[0-9]+(\\.[0-9]+)?"
#define KEYWORDS "null|true|false"
#define EXP "(:|\\[|,)\\s*\"("KEYWORDS"|"NUMBER")\""

namespace json
{
    JSONObject::JSONObject()
    {
        exp = boost::regex(EXP);
    }

    JSONObject::JSONObject(const JSONObject& obj)
    {
        pt = obj.pt;
        exp = boost::regex(EXP);
    }

    JSONObject::JSONObject(std::istream &stream)
    {
        exp = boost::regex(EXP);
        boost::property_tree::read_json(stream, pt);
    }

    JSONObject::JSONObject(const std::string &text)
    {
        exp = boost::regex(EXP);
        std::istringstream ss(text);
        boost::property_tree::read_json(ss, pt);
    }

    JSONObject::JSONObject(const boost::property_tree::ptree &ptChild)
    {
        pt = ptChild;
        exp = boost::regex(EXP);
    }

    std::string JSONObject::getString(const std::string &key) const
    { 
        return pt.get<std::string>(key);
    }

    float JSONObject::getFloat(const std::string& key) const
    {
        return pt.get<float>(key);
    }

    double JSONObject::getDouble(const std::string &key) const
    {
        return pt.get<double>(key);
    }

    int8_t JSONObject::getByte(const std::string& key) const
    {
        return pt.get<uint8_t>(key);
    }

    short JSONObject::getShort(const std::string& key) const
    {
        return pt.get<short>(key);
    }

    int JSONObject::getInt(const std::string &key) const
    {
        return pt.get<int>(key);
    }

    int64_t JSONObject::getLong(const std::string& key) const
    {
        return pt.get<uint64_t>(key);
    }

    bool JSONObject::getBoolean(const std::string &key) const
    {
        return pt.get<bool>(key);
    }

    bool JSONObject::isNull(const std::string &key) const
    {
        bool rv = false;
        std::string value = pt.get<std::string>(key);
        if(value == "null")
        {
            rv = true;
        }
        return rv;
    }

    JSONObject JSONObject::getJSONObject(const std::string &key) const
    {
        JSONObject rv;

        if(!this->isNull(key))
        {
            boost::property_tree::ptree ptChild = pt.get_child(key);
            rv = JSONObject(ptChild);
        }
        
        return rv;
    }

    std::vector<std::string> JSONObject::getArray(const std::string &key) const
    {
        std::vector<std::string> rv;

        if(!this->isNull(key))
        {
            BOOST_FOREACH(const boost::property_tree::ptree::value_type& values, pt.get_child(key))
            {
                rv.push_back(values.second.data());
            }
        }

        return rv;
    }

    std::vector<double> JSONObject::getArrayDouble(const std::string &key) const
    {
        std::vector<std::string> values = getArray(key);
        std::vector<double> rv;
        double number = 0.0;

        if(!this->isNull(key))
        {
            BOOST_FOREACH(std::string& text, values)
            {
                std::istringstream istream(text);
                istream >> number;
                rv.push_back(number);
            }
        }

        return rv;
    }

    std::vector<int> JSONObject::getArrayInt(const std::string &key) const
    {
        std::vector<std::string> values = getArray(key);
        std::vector<int> rv;
        int number = 0;

        if(!this->isNull(key))
        {
            BOOST_FOREACH(std::string& text, values)
            {
                std::istringstream istream(text);
                istream >> number;
                rv.push_back(number);
            }
        }

        return rv;
    }

    std::vector<bool> JSONObject::getArrayBoolean(const std::string &key) const
    {
        std::vector<std::string> values = this->getArray(key);
        std::vector<bool> rv;

        BOOST_FOREACH(std::string& text, values)
        {
            if(text=="true")
                rv.push_back(true);
            else
                rv.push_back(false);
        }

        return rv;
    }

    std::vector<JSONObject> JSONObject::getArrayJSONObject(const std::string &key) const
    {
        std::vector<JSONObject> rv;
 
        if(!this->isNull(key))
        {
            BOOST_FOREACH(const boost::property_tree::ptree::value_type& values, pt.get_child(key))
            {
                JSONObject obj(values.second);
                rv.push_back(obj);
            }
        }

        return rv;
    }

    void JSONObject::put(const std::string& key, const std::string& value)
    {
        pt.put(key, value);
    }

    void JSONObject::put(const std::string& key, const char* value)
    {
        std::string text(value);
        pt.put(key, text);
    }

    void JSONObject::put(const std::string& key, const float value)
    {
        pt.put(key, value);
    }

    void JSONObject::put(const std::string& key, const double value)
    {
        pt.put(key, value);
    }

    void JSONObject::put(const std::string& key, const int8_t value)
    {
        pt.put(key, value);
    }

    void JSONObject::put(const std::string& key, const short value)
    {
        pt.put(key, value);
    }

    void JSONObject::put(const std::string& key, const int value)
    {
        pt.put(key, value);
    }

    void JSONObject::put(const std::string& key, const int64_t value)
    {
        pt.put(key, value);
    }

    void JSONObject::put(const std::string& key, const bool value)
    {
        pt.put(key, value);
    }

    void JSONObject::putNull(const std::string& key)
    {
        pt.put(key, "null");
    }

    void JSONObject::put(const std::string& key, const JSONObject& value)
    {
        pt.put_child(key, value.pt);
    }

    void JSONObject::put(const std::string& key, const std::vector<std::string>& values)
    {
        if(values.size() == 0)
        {
            this->putNull(key);
        }
        else
        {
            boost::property_tree::ptree arrayChild;
            boost::property_tree::ptree arrayElement;

            BOOST_FOREACH(const std::string& text, values)
            {
                arrayElement.put_value(text);
                arrayChild.push_back(std::make_pair("",arrayElement));
            }
            pt.put_child(key, arrayChild);
        }
    }

    void JSONObject::put(const std::string& key, const std::vector<int>& values)
    {
        if(values.size() == 0)
        {
            this->putNull(key);
        }
        else
        {
            boost::property_tree::ptree arrayChild;
            boost::property_tree::ptree arrayElement;

            BOOST_FOREACH(int number, values)
            {
                arrayElement.put_value(number);
                arrayChild.push_back(std::make_pair("",arrayElement));
            }

            pt.put_child(key, arrayChild);
        }
    }

    void JSONObject::put(const std::string& key, const std::vector<double>& values)
    {
        if(values.size() == 0)
        {
            this->putNull(key);
        }
        else
        {
            boost::property_tree::ptree arrayChild;
            boost::property_tree::ptree arrayElement;

            BOOST_FOREACH(double number, values)
            {
                arrayElement.put_value(number);
                arrayChild.push_back(std::make_pair("",arrayElement));
            }

            pt.put_child(key, arrayChild);
        }
    }

    void JSONObject::put(const std::string& key, const std::vector<bool>& values)
    {
        if(values.size() == 0)
        {
            this->putNull(key);
        }
        else
        {
            boost::property_tree::ptree arrayChild;
            boost::property_tree::ptree arrayElement;

            BOOST_FOREACH(bool value, values)
            {
                arrayElement.put_value(value);
                arrayChild.push_back(std::make_pair("",arrayElement));
            }

            pt.put_child(key, arrayChild);
        }
    }

    void JSONObject::put(const std::string& key, const std::vector<JSONObject>& values)
    {
        if(values.size() == 0)
        {
            this->putNull(key);
        }
        else
        {
            boost::property_tree::ptree arrayChild;
            BOOST_FOREACH(JSONObject value, values)
            {
                arrayChild.push_back(std::make_pair("",value.pt));
            }
            pt.put_child(key, arrayChild);
        }
    }

    JSONObject& JSONObject::operator=(const JSONObject& obj)
    {
        if(this != &obj)
        {
            pt = obj.pt;
        }

        return *this;
    }

    std::string JSONObject::toString() const
    {
        std::stringstream ss;
        boost::property_tree::json_parser::write_json(ss, pt);
        std::string rv = boost::regex_replace(ss.str(), exp, "$1$2");
        return rv;
    }

    JSONObject::~JSONObject()
    {
    }

    std::ostream& operator<<(std::ostream& out, const JSONObject& json)
    {   
        return out<<json.toString();
    }
}
