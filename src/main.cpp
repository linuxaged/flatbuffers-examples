#include <cstdio>
#include "flatbuffers/flatbuffers.h"
#include "wall_generated.h"

using namespace mj2::schema;

void Deserialize(char const* buffer)
{
	auto pWall = mj2::schema::GetWall(buffer);
	printf("--dm-- movie id: %d\n", pWall->movieId());

	for (auto sca : *pWall->simulCommentsArray())
	{
		printf("--dm--  rltTime %d\n", sca->rltTime());
		for (auto comment : *sca->comments())
		{
			printf("--dm--   %s, %s\n",
				comment->userId()->c_str(),
				comment->content()->c_str());
		}

		
	}
}

bool UpdateDanmu()
{
	FILE* fp = fopen("comment_wall.bin", "rb");

	//    std::vector<uint8_t> contents;
	if (fp)
	{
		std::fseek(fp, 0, SEEK_END);
		long length = std::ftell(fp);
		char* contents = new char[length];

		std::rewind(fp);
		std::fread(contents, 1, length, fp);
		std::fclose(fp);

		Deserialize(contents);

		return true;
	}
	else
	{
		return false;
	}
}

std::string random_string(size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

int main()
{
	//SetConsoleOutputCP(65001);

	flatbuffers::FlatBufferBuilder fbb;

	// array of comments
	std::vector<flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<mj2::schema::Comment>>>> arrayOfComments;
	for (unsigned n0 = 0; n0 < 3; n0++)
	{
		// Comments
		std::vector<flatbuffers::Offset<Comment>> comment_offsets;
		for (unsigned i = 0; i < 5; i++)
		{
			std::string uid = random_string(4);
			std::string text = random_string(8);
			comment_offsets.push_back(CreateComment(fbb, fbb.CreateString(uid.c_str()), fbb.CreateString(text.c_str())));
		}
		auto vec = fbb.CreateVector(comment_offsets);

		arrayOfComments.push_back( vec );
	}

	
	// array of simul comments
	std::vector<flatbuffers::Offset<mj2::schema::SimulComments>> arrayOfSimulComments;

	unsigned count = 0;
	for (auto& v : arrayOfComments)
	{
		
		arrayOfSimulComments.push_back( CreateSimulComments(fbb, count++, v) );
		count++;
	}

	// SimulComments Vector
	std::vector<flatbuffers::Offset<SimulComments>> simulComment_offsets;
	for (auto& v : arrayOfSimulComments)
	{
		simulComment_offsets.push_back(v);
	}
	
	auto vec_simulComment = fbb.CreateVector(simulComment_offsets);

	auto end = CreateWall(fbb, 12345, 60*60, vec_simulComment);

	fbb.Finish(end);

	FILE* bin = fopen("comment_wall.bin", "wb");
	if (bin)
	{
		fwrite(fbb.GetBufferPointer(), sizeof(char), fbb.GetSize(), bin);
	}
	fclose(bin);

	bool result = UpdateDanmu();

	//auto pWall = mj2::schema::GetWall( fbb.GetBufferPointer() );
	//printf("--dm-- movie id: %s\n", pWall->movieId()->c_str());



	//for (auto comment : *pWall->comments())
	//{
	//	printf("--dm-- %d, %d, %s, %s\n", 
	//		comment->rltTime(),
	//		comment->absTime(),
	//		comment->userId()->c_str(),
	//		comment->content()->c_str());
	//}

	return 0;
}
