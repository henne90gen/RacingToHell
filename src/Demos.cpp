void textDemo(GameMemory* memory, Input* input) {
	static char fontFileNames[3][100] = { "./res/font/DejaVuSansMono.ttf",
			"./res/font/ComicSans.ttf", "./res/font/Arial.ttf" };
	static int fontIndex = 0;

	if (input->enterKeyClicked) {
		loadFont(memory, fontFileNames[fontIndex]);
		fontIndex++;
		if (fontIndex >= 3) {
			fontIndex = 0;
		}
	}

	Math::Rectangle rect = { };
	rect.position = Math::Vector2f(-memory->aspectRatio, 0.5);
	rect.size = Math::Vector2f(memory->aspectRatio * 2.0f, 1.0);
	Render::rectangle(memory, rect, 0x00ff00ff);

	Math::Vector2f position = Math::Vector2f(-1.7f, 0.3);
	Render::text(memory, "The quick brown fox jumps over the lazy dog",
			position, Render::FontSize::Small, 0x0000ffff);

	position = Math::Vector2f(-1.7f, 0);
	Render::text(memory, "The quick brown fox jumps over the lazy dog",
			position, Render::FontSize::Medium, 0x0000ffff);

	position = Math::Vector2f(-1.7f, -0.35);
	Render::text(memory, "The quick brown fox jumps over the lazy dog",
			position, Render::FontSize::Large, 0x0000ffff);
}
