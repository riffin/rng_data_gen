#include <unistd.h>
#include <stdio.h>
#include <random>
#include <string>
#include <iostream>
#include <experimental/filesystem>

using namespace std;
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
		char file_path[8] = { 0 };
		std::random_device device;

		rng_gen_data.value   = device();
		rng_gen_data.unix_ts = time(NULL);

		snprintf(file_path, sizeof(file_path), "%ld",
			 rng_gen_data.unix_ts);

		fp = fopen(file_path, "w+");
		if (!fp) {
			cout << "Failed to open file: " << file_path << endl; 
			goto err;
		}
		fprintf(fp, "%d, %ld", rng_gen_data.value, (long) rng_gen_data.unix_ts);

		fclose(fp);
err:
		return ret;
	}
};

int main()
{
	int ret = EXIT_FAILURE;
	std::string path = "./files";
	std::random_device device;
	std::uniform_int_distribution<int> distribution(500000, 15000000);
	rng_gen rng_gen_obj;

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
