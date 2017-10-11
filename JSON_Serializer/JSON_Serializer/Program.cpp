#include <JSON.h>     
#include <string>

int main()
{
	std::string testStr = "{\"colors\":[\"red\",\"green\",\"blue\"],\"person\":{\"name\":\"Bob\",\"age\":\"20\",\"contact\":{\"email\":\"bob@example.com\"}},\"truthy\":\"True\",\"falsy\":\"False\",\"pi\":\"3.14159\"}";

	JSON::Visualize_data(testStr);

	getchar();
	return 0;
}