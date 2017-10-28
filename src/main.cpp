#include <cstdio>
#include "flatbuffers/flatbuffers.h"
#include "wall_generated.h"

using namespace mj2::schema;

int main()
{
	flatbuffers::FlatBufferBuilder fbb;

	auto str = fbb.CreateString("GoGoGo!");
	auto uid = fbb.CreateString("madi");
	auto content = fbb.CreateString("哇塞哈");

	auto c0 = CreateComment(fbb, 1, 999, uid, content);

	std::vector<flatbuffers::Offset<Comment>> comment_offsets;
	comment_offsets.push_back(c0);
	auto vec = fbb.CreateVector(comment_offsets);

	auto end = CreateWall(fbb, str, vec);

	fbb.Finish(end);

	FILE* bin = fopen("comment_wall.bin", "wb");
	if (bin)
	{
		fwrite(fbb.GetBufferPointer(), sizeof(char), fbb.GetSize(), bin);
		fclose(bin);
	}

	return 0;
}
