#include "local_parameter.h"

void string_methods::split(const std::string& str, const std::string& split, std::vector<std::string>& res)
{
	//std::regex ws_re("\\s+"); // 正则表达式,匹配空格 
	std::regex reg(split);		// 匹配split
	std::sregex_token_iterator pos(str.begin(), str.end(), reg, -1);
	decltype(pos) end;              // 自动推导类型 
	for (; pos != end; ++pos)
	{
		res.push_back(pos->str());
	}
}

std::string string_methods::float_to_str(const float Num)
{
	std::ostringstream oss;
	oss << Num;
	std::string str(oss.str());
	return str;
}

void parameter_file::load_file()
{
	try {
		std::ifstream config_param;
		config_param.open(file_name);
		std::string single_line;
		while (config_param.peek() != EOF)
		{
			config_param >> single_line;
			if (single_line.size() != 0)
			{
				std::vector<std::string> v_str;
				str_methods.split(single_line, ":", v_str);
				assert(v_str.size() == 2);
				parameter[v_str[0]] = v_str[1];
			}
		}
		config_param.close();
	}
	catch (std::exception& e)
	{
		printf("load_file failure: %s \n", e.what());
	}
}

void parameter_file::save_file()
{
	try {
		std::ofstream config_param;
		config_param.open(file_name);
		for (auto itor = parameter.begin(); itor != parameter.end(); itor++)
		{
			config_param << itor->first << ":" << itor->second << std::endl;
		}
		config_param.close();
	}
	catch (std::exception& e)
	{
		printf("save_file failure: %s \n", e.what());
	}
}

void parameter_file::update_file(const std::string& key, const std::string& value)
{
	parameter[key] = value;
}

std::string parameter_file::get_value(const std::string& key)
{
	//如果key不存在，返回默认""
	return parameter[key];
}


void D_XY_base::init(const std::string& name,parameter_file& param)
{
	s = name;
	D_XY_key_str.clear();
	D_XY.clear();
	str_methods.split(s, ",", D_XY_key_str);
	std::lock_guard<std::shared_mutex> lock(mut);
	for (auto itor = D_XY_key_str.begin(); itor != D_XY_key_str.end(); itor++)
	{
		std::string xy_str = param.get_value(*itor);
		D_XY.emplace_back(get_XY_about(xy_str));
	}
}

void D_XY_base::update_to_file(parameter_file& param)
{
	for (int i = 0, len = D_XY.size(); i < len; ++i)
	{
		std::string xy_str = str_methods.float_to_str(D_XY[i].x) + "," + str_methods.float_to_str(D_XY[i].y);
		param.update_file(D_XY_key_str[i], xy_str);
	}
}

void D_XY_base::update(const int i, const cv::Point2f& p)
{
	assert(i < D_XY.size());
	std::lock_guard<std::shared_mutex> lock(mut);
	D_XY[i].x = D_XY[i].x * 0.9 + p.x * 0.1;
	D_XY[i].y = D_XY[i].y * 0.9 + p.y * 0.1;
	printf("New D_XY_key_str[%d] D_XY[%d] :x=%f, y=%f \n", i, i,
		D_XY[i].x, D_XY[i].y);
}

cv::Point2f D_XY_base::get_XY_about(std::string& s)
{
	std::vector<std::string> str_xy;
	str_methods.split(s, ",", str_xy);
	cv::Point2f coordinate(std::atof(str_xy[0].c_str()), std::atof(str_xy[1].c_str()));
	return coordinate;
}