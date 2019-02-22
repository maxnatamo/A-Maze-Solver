#pragma once

typedef struct {
	unsigned long width, height;
} Size;

enum CellTypes {
	Start = 1,
	End = 2,
	Wall = 3,
	Nothing = 4,
	Unknown = 5,
	Path = 6
};
