#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(GetMoveEffect(MOVE_BEAK_BLAST) == EFFECT_BEAK_BLAST);
}

DOUBLE_BATTLE_TEST("Beak Blast's charging message is shown before other moves are used")
{
    GIVEN {
        ASSUME(GetMovePriority(MOVE_BEAK_BLAST) < 0);
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(3); }
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_BEAK_BLAST, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_BEAK_BLAST_SETUP, playerLeft);
        MESSAGE("Wynaut started heating up its beak!");

        MESSAGE("Wobbuffet used Celebrate!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerRight);
        MESSAGE("The opposing Wobbuffet used Celebrate!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentRight);
        MESSAGE("The opposing Wobbuffet used Celebrate!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, opponentLeft);

        MESSAGE("Wynaut used Beak Blast!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BEAK_BLAST, playerLeft);
        HP_BAR(opponentLeft);
    }
}

DOUBLE_BATTLE_TEST("Beak Blast burns all who make contact with the pokemon")
{
    GIVEN {
        ASSUME(GetMovePriority(MOVE_BEAK_BLAST) < 0);
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        PLAYER(SPECIES_WYNAUT) { Speed(10); }
        PLAYER(SPECIES_WOBBUFFET) { Speed(5); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(3); }
        OPPONENT(SPECIES_WOBBUFFET) { Speed(2); }
    } WHEN {
        TURN { MOVE(opponentLeft, MOVE_SCRATCH, target: playerLeft); MOVE(opponentRight, MOVE_SCRATCH, target: playerLeft); MOVE(playerLeft, MOVE_BEAK_BLAST, target: opponentLeft); }
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_BEAK_BLAST_SETUP, playerLeft);
        MESSAGE("Wynaut started heating up its beak!");

        MESSAGE("Wobbuffet used Celebrate!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_CELEBRATE, playerRight);

        MESSAGE("The opposing Wobbuffet used Scratch!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponentLeft);
        HP_BAR(playerLeft);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponentLeft);
        MESSAGE("The opposing Wobbuffet was burned!");
        STATUS_ICON(opponentLeft, burn: TRUE);

        MESSAGE("The opposing Wobbuffet used Scratch!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SCRATCH, opponentRight);
        HP_BAR(playerLeft);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponentRight);
        MESSAGE("The opposing Wobbuffet was burned!");
        STATUS_ICON(opponentRight, burn: TRUE);

        MESSAGE("Wynaut used Beak Blast!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BEAK_BLAST, playerLeft);
        HP_BAR(opponentLeft);
    }
}

SINGLE_BATTLE_TEST("Beak Blast burns only when contact moves are used")
{
    u32 move;
    bool32 burn;
    PARAMETRIZE { move = MOVE_SCRATCH; burn = TRUE; }
    PARAMETRIZE { move = MOVE_WATER_GUN; burn = FALSE; }
    PARAMETRIZE { move = MOVE_LEER; burn = FALSE; }

    GIVEN {
        ASSUME(MoveMakesContact(MOVE_SCRATCH));
        ASSUME(!MoveMakesContact(MOVE_WATER_GUN));
        ASSUME(!MoveMakesContact(MOVE_LEER));
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_BEAK_BLAST); MOVE(opponent, move); }
        TURN {}
    } SCENE {
        ANIMATION(ANIM_TYPE_GENERAL, B_ANIM_BEAK_BLAST_SETUP, player);
        MESSAGE("Wobbuffet started heating up its beak!");
        ANIMATION(ANIM_TYPE_MOVE, move, opponent);

        if (burn) {
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponent);
            MESSAGE("The opposing Wobbuffet was burned!");
            STATUS_ICON(opponent, burn: TRUE);
        }
        else {
            NONE_OF {
                ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_BRN, opponent);
                MESSAGE("The opposing Wobbuffet was burned!");
                STATUS_ICON(opponent, burn: TRUE);
            }
        }

        MESSAGE("Wobbuffet used Beak Blast!");
        ANIMATION(ANIM_TYPE_MOVE, MOVE_BEAK_BLAST, player);
        HP_BAR(opponent);
    }
}

TO_DO_BATTLE_TEST("Beak Blast's charging message is shown regardless if it would've missed");
TO_DO_BATTLE_TEST("Beak Blast fails if it's forced by Encore after choosing a different move");
TO_DO_BATTLE_TEST("Bulletproof is immune to Beak Blast but not to the burn it causes");
