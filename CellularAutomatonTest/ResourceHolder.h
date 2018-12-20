#ifndef RESOURCEHOLDER_H_INCLUDED
#define RESOURCEHOLDER_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "ResourceManager.h"
#include "./NonCopyable.h"
#include "./NonMoveable.h"

class ResourceHolder : public NonCopyable, public NonMovable
{
public:
	static ResourceHolder& get();

	ResourceManager<sf::Font>           fonts;
	ResourceManager<sf::Texture>        textures;

private:
	ResourceHolder();
};

#endif
