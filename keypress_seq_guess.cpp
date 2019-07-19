#include "keypress_seq_guess.h"

/**
 * API Name: gen_random_seq()
 * Arguments: none
 * Retun: void
 * Description: This function will generate random sequence.
 */
void Controls::Sequence_Guess::gen_random_seq()
{
    //Generate a random seq from 1 to 3
    m_random_seq.first = static_cast<Inputs::Button_t>(rand() % 3 + 1);
    m_random_seq.second = static_cast<Inputs::Button_t>(rand() % 3 + 1);
    m_random_seq.third = static_cast<Inputs::Button_t>(rand() % 3 + 1);
    std::cout << "New Sequence generated, try your guess" << std::endl;
}

/**
 * API Name: init()
 * Arguments: none
 * Retun: void
 * Description: This function will initialize the class by
 * generating random sequence and initializing keypres thread.
 */
void Controls::Sequence_Guess::init()
{
    //Initialliy, generate a random sequence
    gen_random_seq();

    //make seq is not ready for user input
    m_is_seq_ready = false;

    //Start User input thread
    m_kp = std::thread(Inputs::keypress, std::ref(m_usr_key_seq), std::ref(m_is_seq_ready));
}

/**
 * API Name: proc_and_display()
 * Arguments: none
 * Retun: if all LEDs are green, this function returns true; else false
 * Description: This function will process the user input in comparision 
 * with random generated sequence and displays results into stdout.
 */
bool Controls::Sequence_Guess::proc_and_display()
{
    bool ret = false; //return
    Outputs::Led_Color_t led1_color, led2_color, led3_color;
    
    //Check first entry in sequence
    if(m_random_seq.first == m_usr_key_seq.first)
    {
        led1_color = Outputs::LEDCOLOR_GREEN;
    }
    else if ((m_random_seq.first == m_usr_key_seq.second) || (m_random_seq.first == m_usr_key_seq.third))
    {
        led1_color = Outputs::LEDCOLOR_ORANGE;
    }
    else
    {
        led1_color = Outputs::LEDCOLOR_RED;
    }
    
    //Check second entry in sequence
    if(m_random_seq.second == m_usr_key_seq.second)
    {
        led2_color = Outputs::LEDCOLOR_GREEN;
    }
    else if ((m_random_seq.second == m_usr_key_seq.first) || (m_random_seq.second == m_usr_key_seq.third))
    {
        led2_color = Outputs::LEDCOLOR_ORANGE;
    }
    else
    {
        led2_color = Outputs::LEDCOLOR_RED;
    }

    //Check third entry in sequence
    if(m_random_seq.third == m_usr_key_seq.third)
    {
        led3_color = Outputs::LEDCOLOR_GREEN;
    }
    else if ((m_random_seq.third == m_usr_key_seq.first) || (m_random_seq.third == m_usr_key_seq.second))
    {
        led3_color = Outputs::LEDCOLOR_ORANGE;
    }
    else
    {
        led3_color = Outputs::LEDCOLOR_RED;
    }

    //finilazing return statement
    ret = ((led1_color == Outputs::LEDCOLOR_GREEN) && (led1_color == led2_color) && (led2_color == led3_color))? true:false;
    if(!ret)
        std::cout << "Oops wrong ! try again" <<std::endl;

    //time to output the color
    Outputs::indicate_color(Outputs::LED_1, led1_color);
    Outputs::indicate_color(Outputs::LED_2, led2_color);
    Outputs::indicate_color(Outputs::LED_3, led3_color);

    return ret;
}

/**
 * API Name: run()
 * Arguments: none
 * Retun: on success this function returns Zero
 * Description: This is the core engine of the game.
 * User shall call this function once to run the game.
 */
int32_t Controls::Sequence_Guess::run()
{
    //Initialize first
    init();

    auto game_control = [&]() {
        while(true) {
            std::unique_lock<std::mutex> ctrl_lck(m_ctrl_lck);
            //Let's wait until user input sequence is ready to process
            while(!m_is_seq_ready) 
            {
                Inputs::c_seq_ready.wait(ctrl_lck);
            }
            //Process user input and display the result
            //If all LEDs are green, let's generate new sequence
            if( proc_and_display() == true ) 
            {
                std::cout << "Right ! Nice guess !!" << std::endl;
                gen_random_seq();
            }

            //let's get ready for next user inputs
            m_is_seq_ready = false;
        }
    };

    //Let's run the game
    m_ctrl = std::thread(game_control);

    m_ctrl.join(); //let's hope it will join some time
    m_kp.join(); //let's hope it will join some time

    return 0; //finally all success !!!
}

/**
 * API Name: main()
 * Arguments: none
 * Retun: on success this function returns Zero
 * Description: This is the main function call.
 */
int main()
{
    Controls::Sequence_Guess game;
    return game.run();
}