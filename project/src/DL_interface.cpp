#include "DL_interface.h"



void DL_module::load()
{
	try
	{
		model = torch::jit::load(path);
		if (use_cuda)
			model.to(at::kCUDA);
		model.clone();
	}
	catch (std::exception& e)
	{
		printf("模型加载异常 \n");
		exit(1);
	}
}

void DL_module::clone(DL_module& m)
{
	std::lock_guard<std::mutex> lock(mut);
	m.model = model.clone();
	m.path = path;
	m.use_cuda = use_cuda;
}

void DL_module::single_interface(cv::Mat& image, std::pair<int, float>& class_and_probability)
{
	image.convertTo(image, CV_32FC3, 1.0f / 255.0f);  // step2. 归一化
			// step3. 转tensor
	int img_h = image.rows;
	int img_w = image.cols;
	int depth = image.channels();
	auto input_tensor = torch::from_blob(image.data, { 1, img_h, img_w, depth });
	input_tensor = input_tensor.permute({ 0, 3, 1, 2 });  // step4. 通道转换
	input_tensor[0][0].sub_(0.5).div_(0.25);  //減去均值,除以標準差
	//input_tensor[0][1].sub_(0.5).div_(0.25);
	//input_tensor[0][2].sub_(0.5).div_(0.25);

	std::vector<torch::jit::IValue> inputs;
	if (use_cuda)
	{
		inputs.push_back(input_tensor.to(at::kCUDA));
	}
	else
	{
		inputs.push_back(input_tensor);
	}

	// inference
	torch::NoGradGuard no_grad;
	at::Tensor output = model.forward(inputs).toTensor();
	at::Tensor softmax_output = torch::softmax(output, 1);
	std::vector<float> output_v(output.data_ptr<float>(), output.data_ptr<float>() + output.numel());
	std::vector<float> softmax_output_v(softmax_output.data_ptr<float>(), softmax_output.data_ptr<float>() + softmax_output.numel());
	float image_class = 0, image_class_value = softmax_output_v[0];
	for (int i = 0, length = output_v.size(); i < length; ++i)
	{
		//printf("output_v[%d] = %f, softmax_output_v[%d] = %f\n", i, output_v[i], i, softmax_output_v[i]);
		if (image_class_value < softmax_output_v[i])
		{
			image_class = i;
			image_class_value = softmax_output_v[i];
		}
	}
	//printf("image class: %f, value: %f", image_class, image_class_value);
	class_and_probability.first = image_class;
	class_and_probability.second = image_class_value;
}

