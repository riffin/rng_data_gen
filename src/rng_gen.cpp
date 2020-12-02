#include <unistd.h>
#include <stdio.h>
#include <random>
#include <string>
#include <iostream>
#include <thread>
#include <fstream>
#include <experimental/filesystem>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using namespace std;
using namespace curlpp::options;

namespace fs = std::experimental::filesystem;

typedef struct {
	int value;
	time_t unix_ts;
} rng_gen_data_t;

class rng_gen {
    private:
	rng_gen_data_t rng_gen_data;

    public:
	int stash_data()
	{
		FILE *fp;
		int ret = -1;
		char file_path[16] = { 0 };
		std::random_device device;

		rng_gen_data.value   = device();
		rng_gen_data.unix_ts = time(NULL);

		snprintf(file_path, sizeof(file_path), "./files/%ld",
			 rng_gen_data.unix_ts);

		fp = fopen(file_path, "w+");
		if (!fp) {
			cout << "Failed to open file: " << file_path << endl; 
			goto err;
		}
		fprintf(fp, "%d, %ld", rng_gen_data.value, (long) rng_gen_data.unix_ts);

		fclose(fp);
		ret = 0;
err:
		return ret;
	}
};

class post_data {
   private:
	std::string read_file(std::ifstream& in) {
		std::ostringstream sstr;
		sstr << in.rdbuf();
		return sstr.str();
	}

	int handle_files(string *file_info)
	{
		std::ifstream ifs;
		std::string path = "./files";

		for (const auto & entry : fs::directory_iterator(path)) {
			ifs.open(entry.path(), std::ifstream::in);
			file_info->append(read_file(ifs));
			ifs.close();

			fs::remove(entry.path());
		}

		return 0;
	}

   public:
	int perform_post()
	{
		string file_info;
		char buf[50] = { 0 };
		curlpp::Cleanup cleaner;
		curlpp::Easy request;
		std::list<std::string> headers;

		handle_files(&file_info);

		request.setOpt<Url>("http://127.0.0.1:8080/");
		request.setOpt(new curlpp::options::Verbose(true));

		headers.push_back("Content-Type: text/*");
		sprintf(buf, "Content-Length: %ld", file_info.size());
		headers.push_back(buf);

		request.setOpt(new curlpp::options::HttpHeader(headers));

		request.setOpt(new curlpp::options::PostFields(file_info));
		request.setOpt(new curlpp::options::PostFieldSize(file_info.size()));

		request.perform();

		return 0;
	}
};

void handle_post()
{
	post_data post_handler;

	while (1) {
		post_handler.perform_post();
		sleep(60);
	}
}

int main()
{
	int ret = EXIT_FAILURE;
	std::random_device device;
	std::uniform_int_distribution<int> distribution(500000, 15000000);
	rng_gen rng_gen_obj;

	fs::create_directory("./files/");

	std::thread first (handle_post);

	while (1) {
		if (rng_gen_obj.stash_data()) {
			cout << "Failed to stash data" << endl;
			goto err;
		}
		usleep(distribution(device));
	}

	ret = EXIT_SUCCESS;

err:
	return ret;
}
