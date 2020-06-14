#pragma once

class Match;
class PhaseHandler {
   private:
   public:
    Match& match;

    PhaseHandler(Match& match);

    // Call the appropriate ${match} function
    // according to its ${currentPhase}
    void processPhase();
};
