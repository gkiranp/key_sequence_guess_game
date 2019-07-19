#ifndef KEYPRESS_SEQ_GUESS_H_
#define KEYPRESS_SEQ_GUESS_H_

#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>

namespace Inputs 
{
    enum Button_t { BTN_A = 1, BTN_B = 2, BTN_C = 3 };
    //Condition variable to keep track of sequence is ready to process
    std::condition_variable c_seq_ready;

    typedef struct _key_seq_t
    {
        Button_t first;
        Button_t second;
        Button_t third;
    } KeySeq_t;

    //This API helps to validate button press event
    auto validate_key = [](const char ip) {
        if(ip == 'A' || ip == 'a') {
            return BTN_A;
        }
        else if (ip == 'B' || ip == 'b') {
            return BTN_B;
        }
        else if (ip == 'C' || ip == 'c') {
            return BTN_C;
        }
        else
        {
            return static_cast<Button_t>(0);
        }
    };

    //Thread :: This API will receive 3 seq inputs from user
    auto keypress = [&c_seq_ready](KeySeq_t &key_seq, bool &proc_ready) 
    {
        std::mutex mu_keypress;
        char in[4] = {0,0,0,0};
        std::chrono::milliseconds timeout(500);
		while (true)
		{
            std::unique_lock<std::mutex> key_lock(mu_keypress);
            memset(in, 0, 4);
            std::cout << "Enter your inputs : ";
            std::cin.getline(in, 4);
            key_seq.first = validate_key(in[0]);  //one before 
            key_seq.second = validate_key(in[1]); //one before most recent
            key_seq.third = validate_key(in[2]);  //most recent keypress
            std::cin.clear();
            //Flag all are OK and
            proc_ready = true;
            //Let's tell controller to process these key inputs
            c_seq_ready.notify_one();
            //Let's wait and relax for a while
            std::this_thread::sleep_for(timeout);
		}
    };

} //end-of-Inputs

namespace Outputs
{
    //LED types
    enum Led_t { LED_1 = 1, LED_2 = 2, LED_3 = 3 };
    //LED Color types
    enum Led_Color_t { LEDCOLOR_RED = 1, LEDCOLOR_ORANGE = 2, LEDCOLOR_GREEN = 3 };

    //This API helps to covert Led_color enum to corresponding string
    auto led_color_str = [](const Led_Color_t &color) {
        switch(static_cast<int32_t>(color))
        {
            case static_cast<int32_t>(LEDCOLOR_RED):
                return "RED";
            break;
            case static_cast<int32_t>(LEDCOLOR_ORANGE):
                return "ORANGE";
            break;
            case static_cast<int32_t>(LEDCOLOR_GREEN):
                return "GREEN";
            break;
            default:
            break;
        }
    };

    //This API is used to output the color of LEDs to stdout
    auto indicate_color = [](const Led_t &led, const Led_Color_t &clr)
    {
        std::mutex mu_stdout;
        std::unique_lock<std::mutex> key_lock(mu_stdout);
        std::cout << "LED_" << static_cast<int32_t>(led) << " : " << led_color_str(clr) << std::endl;
    };
} //end-of-Outputs

namespace Controls 
{
    class Sequence_Guess
    {
    public:
        int32_t run();

    private:
        Inputs::KeySeq_t m_random_seq;
        Inputs::KeySeq_t m_usr_key_seq;
        bool m_is_seq_ready;
        std::mutex m_ctrl_lck;
        std::thread m_kp;
        std::thread m_ctrl;

    private:
        void init();
        void gen_random_seq();
        bool proc_and_display();
    };
} //end-of-Controls


#endif //end of KEYPRESS_SEQ_GUESS_H_