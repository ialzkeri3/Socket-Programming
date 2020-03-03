import java.util.*;
import java.net.*;
import java.io.*;


public class client {


    static ArrayList<Character> Board = new ArrayList<>();
    static char[] WrongLetters = new char[6];
    static int NumWrongLetters = 0;
    static int PrevNumWrongLetters = 0;
    static int WordLength = 0;
    static char prevChar;
    static int WrongLetterIndex = 0;

    public static void main(String args[])
    {
        int argc = args.length;

        if (argc != 2) {
            System.out.println("Error: input should be of the form \njava client <IP address> <port number>");
            System.exit(1);
        }
        try {

            Scanner RawScr = new Scanner(System.in);
            System.out.print("$ Ready to start the game? (y/n):");
            char sndmsg = RawScr.nextLine().charAt(0);

            while(sndmsg != 'y' && sndmsg != 'n'){
                System.out.print("$ Ready to start the game? (y/n):");
                sndmsg = RawScr.nextLine().charAt(0);
            }

            if(sndmsg == 'n'){
                System.exit(0);

            }
            else{
                Socket socket = new Socket(args[0], Integer.parseInt(args[1]));
                send_msg_pkt(socket,"");
                System.out.println();
                //receive is a method that receives server messages and print the board and wrong letters
                while(receive(socket)) {
                    if (Board.contains('_') && (WrongLetters[5] == '\u0000')) {
                        System.out.print("$ Letter to guess: ");
                        String guess = RawScr.nextLine().toLowerCase();
                        //isValidGuess is a method that returns false if the user entered an invalid input
                        while (!isValidGuess(guess)) {
                            System.out.print("$ Letter to guess: ");
                            guess = RawScr.nextLine().toLowerCase();
                        }
                        prevChar = guess.charAt(0);
                        send_msg_pkt(socket, guess);

                    }
                }

                socket.close();

                System.exit(0);

            }
        }
        catch (Exception e){
            System.out.println("Error: "+e);
            System.exit(1);
        }
    }
    static boolean isValidGuess(String guess){
        if(guess.length() > 1){
            System.out.println("$ Error! Please guess one letter.");
            return false;
        }
        char CharGuess = guess.charAt(0);
        if (!(CharGuess >= 'a' && CharGuess <= 'z')){
            System.out.println("$ Error! Please guess one letter.");
            return false;
        }
        for (int i: WrongLetters) {
            if(i == CharGuess || Board.contains(CharGuess)){
                System.out.println("$ Error! Letter "+CharGuess+ " has been guessed before, please guess another letter.");
                return false;
            }
        }
        return true;
    }

    static void send_msg_pkt(Socket s, String data) throws IOException{



        PrintWriter out =
                new PrintWriter(s.getOutputStream(), true);

        //String msg_flag = "";
        //msg_flag += data.length();
        //out.println(msg_flag+data);

        char msgFlag = (char) (data.length()+48);

        String packet = msgFlag + data;

        out.println(packet);




    }

    static boolean receive(Socket s) throws IOException{


        BufferedReader in =
                new BufferedReader(
                        new InputStreamReader(s.getInputStream()));
        String msg = in.readLine();


        if( msg.charAt(0) != '0' ){
            System.out.println("$ "+msg.substring(1));
            return false;
        }

        WordLength = Integer.parseInt(msg.substring(1,2));
        NumWrongLetters = Integer.parseInt(msg.substring(2,3));



        if(NumWrongLetters > PrevNumWrongLetters){
            WrongLetters[WrongLetterIndex] = prevChar;
            PrevNumWrongLetters++;
            WrongLetterIndex++;
        }
        Board.clear();

        for(int i = 3; i < msg.length();i++)
            Board.add(msg.charAt(i));

        System.out.print("$ ");

        for (char i: Board) {
            System.out.print(i);
            System.out.print(" ");
        }
        System.out.println();

        System.out.print("$ Incorrect guesses: ");
        for (int i = 0; i < WrongLetterIndex ;i++) {
            System.out.print(WrongLetters[i]);
            System.out.print(" ");
        }
        System.out.println("\n$");
        return true;
    }


}
