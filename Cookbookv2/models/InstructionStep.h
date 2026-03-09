#pragma once

#include <QString>

struct InstructionStep
{
    int id = -1;
    int recipeId = -1;

    int stepNumber = 0;
    QString instruction;
};
