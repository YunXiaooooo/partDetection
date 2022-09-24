#pragma once
#ifndef LOCAL_PAMAMTER_H
#define LOCAL_PAMAMTER_H
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <regex>
#include <mutex>
#include <shared_mutex>

class string_methods
{
public:
	string_methods()
	{
	}
	~string_methods()
	{
	}
	string_methods(string_methods&) = delete;
	string_methods& operator = (const string_methods&) = delete;
	void split(const std::string& str, const std::string& split, std::vector<std::string>& res);
	std::string float_to_str(const float Num);
private:

};

class parameter_file
{
public:
	parameter_file(const std::string& str)
	{
		init(str);
	}
	parameter_file()
	{
	}
	~parameter_file()
	{
	}
	parameter_file(parameter_file&) = delete;
	parameter_file& operator=(const parameter_file&) = delete;
	void init(const std::string& str)
	{
		file_name = str;
		load_file();
	}
	void save_file();
	void update_file(const std::string& key, const std::string& value);
	std::string get_value(const std::string& key);
private:
	void load_file();
	std::string file_name;
	std::map<std::string, std::string> parameter;
	string_methods str_methods;

};

class D_XY_base
{
public:
	D_XY_base(parameter_file& param, const std::string& name)
	{
		init(name, param);
	}
	D_XY_base()
	{
	}
	~D_XY_base()
	{
	}
	D_XY_base(const D_XY_base&) = delete;
	D_XY_base& operator=(const D_XY_base&) = delete;

	void init(const std::string& name, parameter_file& param);
	void update_to_file(parameter_file& param);
	unsigned int size() const
	{
		return static_cast<unsigned int>(D_XY.size());
	}
	cv::Point2f read(const int i) const
	{
		std::shared_lock<std::shared_mutex> lock(mut);
		return D_XY[i];
	}
	void update(const int i, const cv::Point2f& p);

private:
	std::vector<cv::Point2f> D_XY;
	std::vector<std::string> D_XY_key_str;
	string_methods str_methods;
	std::string s;
	//std::array<cv::Point2f, 4> contacts_holes_base_coordinates{ cv::Point2f(327,141),cv::Point2f(305,586), cv::Point2f(326,796), cv::Point2f(307,1236) };
	//std::array<cv::Point2f, 2> single_contact_holes_base_coordinates{ cv::Point2f(341,117),cv::Point2f(342,512) };
	mutable std::shared_mutex mut;
	cv::Point2f get_XY_about(std::string& s);
};


#endif