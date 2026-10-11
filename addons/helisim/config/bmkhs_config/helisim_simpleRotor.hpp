/////////////////////////////////////////////////////////////////////////////////////////////
// Rotors - Simple //////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//TODO(BO105): the tail rotor tables are the Tiger's, scaled - fit them in the hover.
//A rotor is a hub at a position turning blades of a given size, so its geometry, its blade
//and what it does with the controls are defined together here. Core reads numSimpleRotors
//and loops; nothing downstream indexes by rotor NUMBER.
//
//The simple model works four fixed blade positions and scales by blade count, so the lift
//and drag tables carry what the rotor does rather than deriving it per blade element.
//
//  type         - "main" or "tail". What the rotor IS, so Core never assumes rotor 0 is
//                 the main one.
//  direction    - "ccw" or "cw", seen from above.
//  numBlades    - the four modelled positions are scaled to this.
//  pivot[]      - hub position, {lateral, longitudinal, vertical} in m,
//                 right-positive / nose-positive / up-positive
//  rotation[]   - disc orientation, {pitch, roll, yaw} in deg
//  mastLength   - m along the disc's own up axis, from pivot to hub
//  gearRatio    - rotor to engine shaft; shared with the transmission model
//  torqueTau    - s, torque filter time constant
//  damageRole   - the hitpoint role that destroys this rotor. A rotor that declares none
//                 cannot be destroyed.
//
//  BLADE
//  bladeRadius  - m
//  bladeChord   - m
//  bladeMass    - kg, one blade
//
//  DISC TILT - min / mid / max in deg, interpolated from the centred stick. A rotor whose
//  disc does not tilt declares zeroes.
//
//  coneAngle    - deg at full collective. Coning lifts the tips, so the thrust position
//                 moves inboard and the disc carries a vertical arm.
//  flapBackRollMax / flapBackPitchMax - deg at an advance ratio of 1.0. The advancing blade
//                 lifts more than the retreating one, so the disc tilts as speed builds.
//                 Applied as blade flap, which moves both the thrust position and its
//                 direction - it is NOT also applied to the lift coefficient.
//  rollLiftCoef / pitchLiftCoef - the cyclic lift coefficient, independent of the
//                 collective's. This is what makes the fore/aft and left/right blades carry
//                 different lift, so the pitch and roll moments come out of real forces at
//                 real positions rather than being applied as a torque.
//  gndEffValue  - thrust multiplier on the deck, fading to 1.0 by one rotor diameter up.
//                 A rotor that does not sit in ground effect declares 1.0.
//  reacTqScalar - scales the tangential blade drag that produces the yaw reaction. The same
//                 drag drives the transmission, which this does not touch.
//
//  liftCoefTable / dragCoefTable - rows are the control axis that loads this rotor
//  (collective for a main, pedal for a tail), columns are the airspeeds in the header row,
//  m/s. The drag table carries induced and profile together, and its airspeed columns carry
//  how they vary with speed - that is what the transmission feels.

    numSimpleRotors = 2;
    //Rotor limits, Nr - {normal low, normal high, high rotor, maximum}; below and above normal is transient.
    nrLimits[] = {0.98, 1.02, 1.05, 1.10};
    class SimpleRotors {
        class SimpleRotor01 {
            type             = "main";
            direction        = "ccw";
            damageRole       = "mainRotor";
            numBlades        = 4;
            pivot[]          = {0.000, 0.810, 0.170};
            rotation[]       = {0.00, 0.00, 0.00};
            mastLength       = 0.66;      //m
            gearRatio        = 14.151;    //6000 / 424 rpm
            torqueTau        = 0.10;      //s

            bladeRadius      = 4.920;     //m
            bladeChord       = 0.450;     //m
            bladeMass        = 25.000;    //kg

            pitchFlapMin     = -10.0;     //deg
            pitchFlapMid     =   0.0;
            pitchFlapMax     =  20.0;
            rollFlapMin      = -10.5;
            rollFlapMid      =   0.0;
            rollFlapMax      =   7.0;

            coneAngle        = 12.0;  //deg at full collective
            flapBackRollMax  = 15.0;  //deg per unit advance ratio
            flapBackPitchMax =  9.0;  //deg per unit advance ratio
            rollLiftCoef     = 0.166;
            pitchLiftCoef    = 0.315;
            gndEffValue      = 1.225;
            reacTqScalar     = 0.50;
            autoTorque       = 80.0;

            //rotortables.py --cg 0 1.096 -0.218 --hover 1950:63.6 2460:86 3179:120 --me 60:38.2 --mr 118.4 --cruise-pitch -5
            liftCoefTable[] = {
                        {"A/S", 0.00, 4.54, 9.08, 13.62, 18.16, 22.70, 27.24, 30.87, 31.70, 35.87, 40.05, 44.22, 48.39, 52.56, 56.74, 60.91, 65.26, 69.61, 92.60}
                        ,{0.000, 0.0231, 0.0250, 0.0289, 0.0333, 0.0372, 0.0391, 0.0412, 0.0416, 0.0417, 0.0422, 0.0411, 0.0386, 0.0361, 0.0331, 0.0305, 0.0281, 0.0262, 0.0253, 0.0208}
                        ,{0.850, 0.2178, 0.2360, 0.2726, 0.3139, 0.3513, 0.3687, 0.3886, 0.3920, 0.3929, 0.3984, 0.3878, 0.3646, 0.3406, 0.3122, 0.2878, 0.2655, 0.2470, 0.2389, 0.1959}
                        ,{0.900, 0.2084, 0.2259, 0.2609, 0.3004, 0.3362, 0.3528, 0.3719, 0.3752, 0.3760, 0.3813, 0.3711, 0.3489, 0.3260, 0.2988, 0.2754, 0.2541, 0.2364, 0.2286, 0.1875}
                        ,{0.950, 0.1923, 0.2084, 0.2407, 0.2772, 0.3102, 0.3255, 0.3432, 0.3461, 0.3470, 0.3518, 0.3424, 0.3219, 0.3008, 0.2757, 0.2541, 0.2345, 0.2181, 0.2109, 0.1730}
                        ,{1.000, 0.1668, 0.1808, 0.2088, 0.2404, 0.2691, 0.2824, 0.2977, 0.3003, 0.3010, 0.3052, 0.2970, 0.2793, 0.2609, 0.2392, 0.2204, 0.2034, 0.1892, 0.1830, 0.1501}
                        };
            dragCoefTable[] = {
                        {"A/S", 0.00, 4.54, 9.08, 13.62, 18.16, 22.70, 27.24, 30.87, 31.70, 35.87, 40.05, 44.22, 48.39, 52.56, 56.74, 60.91, 65.26, 69.61, 92.60}
                        ,{0.000, 0.0037, 0.0037, 0.0036, 0.0035, 0.0035, 0.0035, 0.0035, 0.0035, 0.0035, 0.0035, 0.0035, 0.0035, 0.0035, 0.0035, 0.0035, 0.0035, 0.0036, 0.0037, 0.0037}
                        ,{0.476, 0.0156, 0.0154, 0.0152, 0.0150, 0.0148, 0.0148, 0.0148, 0.0148, 0.0148, 0.0148, 0.0148, 0.0148, 0.0148, 0.0148, 0.0148, 0.0148, 0.0152, 0.0156, 0.0156}
                        ,{0.629, 0.0215, 0.0212, 0.0209, 0.0206, 0.0204, 0.0204, 0.0204, 0.0204, 0.0204, 0.0204, 0.0204, 0.0204, 0.0204, 0.0204, 0.0204, 0.0204, 0.0209, 0.0215, 0.0215}
                        ,{0.850, 0.0302, 0.0299, 0.0294, 0.0289, 0.0287, 0.0287, 0.0287, 0.0287, 0.0287, 0.0287, 0.0287, 0.0287, 0.0287, 0.0287, 0.0287, 0.0287, 0.0295, 0.0302, 0.0302}
                        ,{0.900, 0.0393, 0.0388, 0.0382, 0.0376, 0.0373, 0.0373, 0.0373, 0.0373, 0.0373, 0.0373, 0.0373, 0.0373, 0.0373, 0.0373, 0.0373, 0.0373, 0.0383, 0.0393, 0.0393}
                        ,{0.950, 0.0544, 0.0538, 0.0529, 0.0521, 0.0517, 0.0517, 0.0517, 0.0517, 0.0517, 0.0517, 0.0517, 0.0517, 0.0517, 0.0517, 0.0517, 0.0517, 0.0530, 0.0544, 0.0544}
                        ,{1.000, 0.0786, 0.0777, 0.0763, 0.0752, 0.0746, 0.0746, 0.0746, 0.0746, 0.0746, 0.0746, 0.0746, 0.0746, 0.0746, 0.0746, 0.0746, 0.0746, 0.0766, 0.0786, 0.0786}
                        };
        };

        class SimpleRotor02 {
            type             = "tail";
            direction        = "ccw";
            damageRole       = "tailRotor";
            numBlades        = 2;
            pivot[]          = {-1.000, -5.177, 0.975};
            rotation[]       = {0.00, -90.00,  0.000};
            mastLength       = -0.56;     //m
            gearRatio        = 2.704;     //6000 / 2219 rpm
            torqueTau        = 0.10;      //s

            bladeRadius      = 0.950;     //m
            bladeChord       = 0.180;     //m
            bladeMass        = 1.490;     //kg

            //The tail disc does not tilt - pedal changes its pitch, not its plane.
            pitchFlapMin     = 0.0;
            pitchFlapMid     = 0.0;
            pitchFlapMax     = 0.0;
            rollFlapMin      = 0.0;
            rollFlapMid      = 0.0;
            rollFlapMax      = 0.0;

            coneAngle        = 0.0;
            flapBackRollMax  = 0.0;
            flapBackPitchMax = 0.0;
            rollLiftCoef     = 0.0;
            pitchLiftCoef    = 0.0;
            gndEffValue      = 1.0;
            reacTqScalar     = 0.25;
            autoTorque       = 0.0;

            //Pedal through controlMap is the table key; rows are left / mid / right. The main
            //rotor turns counter-clockwise, so left pedal carries the torque.
            controlMap[] = {
                {-1.00, -1.0000},
                {-0.75, -0.8875},
                {-0.50, -0.6250},
                {-0.25, -0.2700},
                { 0.00,  0.0000},
                { 0.25,  0.3900},
                { 0.50,  0.7000},
                { 0.75,  0.8900},
                { 1.00,  1.0000}
            };
            //-----------Pedal----0.00---10.29---20.58---36.01---46.30---51.44---61.73---66.88---72.02
            liftCoefTable[] = {
                         {"A/S", 0.00,  10.29,  20.58,  36.01,  46.30,  51.44,  61.73,  66.88,  72.02}
                        ,{-1.00, -1.1648, -1.2803, -1.3821, -1.5163, -1.5966, -1.6346, -1.7071, -1.7415, -1.7750}
                        ,{ 0.00, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000}
                        ,{ 1.00,  1.1648,  1.2803,  1.3821,  1.5163,  1.5966,  1.6346,  1.7071,  1.7415,  1.7750}
                        };
            //-----------Pedal----0.00---10.29---20.58---36.01---46.30---51.44---61.73---66.88---72.02
            dragCoefTable[] = {
                         {"A/S", 0.00,  10.29,  20.58,  36.01,  46.30,  51.44,  61.73,  66.88,  72.02}
                        ,{-1.00, 0.2519, 0.2519, 0.2519, 0.2519, 0.2519, 0.2519, 0.2519, 0.2519, 0.2519}
                        ,{ 0.00, 0.0273, 0.0273, 0.0273, 0.0273, 0.0273, 0.0273, 0.0273, 0.0273, 0.0273}
                        ,{ 1.00, 0.0084, 0.0084, 0.0084, 0.0084, 0.0084, 0.0084, 0.0084, 0.0084, 0.0084}
                        };
        };
    };
