import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketAddress;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Scanner;
import java.util.Random;

class myThread extends Thread{

    Socket c;
    SocketAddress cA;


    public void run(){

        try {

            server.newClient(c, cA);

        }catch (IOException e){
            System.out.println("Error");
        }
    }

    public void initilize(Socket client, SocketAddress cAddress) throws IOException{

        c =  client;
        cA = cAddress;

    }


}
class Game {

    public String word;
    public char[] board;
    public String wrongLetters;

    public boolean isWon;
    public boolean isOver;

    public Game(String w) {

        this.word = w.toLowerCase();
        wrongLetters = "";

        board = new char[word.length()];

        for(int i=0 ; i<word.length() ; i++)
            board[i] = '_';

        isWon = false;
        isOver = false;

    }

    public void guess(String letter){

        if(word.contains(letter)){
            int[] matchBoard = matching(letter.charAt(0));
            for(int i = 0; i<matchBoard.length ;i++){
                if (matchBoard[i] != -1)
                    board[i] = word.charAt(i);
                if (!(new String(board).contains("_"))) {
                    isWon = true;
                    isOver = true;
                }
            }
        }
        else{
            wrongLetters += letter;
            if(wrongLetters.length() == 6)
                isOver = true;
        }


    }

    private int[] matching(char letter){

        int[] match = new int[word.length()];

        for(int i=0 ; i<word.length() ; i++)
            match[i] = -1;

        for(int i=0 ; i<word.length() ; i++) {
            if(word.charAt(i) == letter) {
                match[i] = i;
            }
        }

        return match;

    }
}


public class server {
    static String[] words = {"amount","voice","oven","snake","umbrella","volcano",
            "request","month","slope","bite","form","trucks","man","building","toys"};
    static int runningClients;

    public static void main(String[] args) throws java.net.UnknownHostException{

        runningClients = 0;
        int port = 9012;
        InetAddress addr = InetAddress.getByName("127.0.0.1");


        if (args.length != 0 && args.length != 2) {
            System.out.println("Error: input should be of the form \njava server <IP Address> <port number>");
            System.exit(0);
        }

        if (args.length == 2) {
            addr = InetAddress.getByName(args[0]);
            System.out.println(addr);
            port = Integer.parseInt(args[1]);
        }

        try {


            ServerSocket server;

            //System.out.println("Starting sever under the port " + port);
            server = new ServerSocket(port,0,addr);


            while (true) {
                //System.out.println("Waiting for connections...");
                Socket client = server.accept();
                SocketAddress clientAddr = client.getLocalSocketAddress();
                //System.out.println("Connection stablished with address: ."+ clientAddr);
                //System.out.println("Attending requisitions.");

                myThread t = new myThread();
                t.initilize(client,clientAddr);
                t.start();
            }
        } catch (IOException e) {
            System.out.println("Failed starting server.");
            System.out.println("Shutting down...");
            System.exit(0);
        }
    }

    public static void newClient(Socket client, SocketAddress clientAddr) throws java.io.IOException{

        //sendMsg(client, "Ready to start game? (y/n): ");

        String startGameMsg = receiveMsg(client);

//        char flag = startGameMsg.charAt(0);
//        int f = flag;

        if(startGameMsg.charAt(0) == '0'){
            if(runningClients >= 3)
                sendMsgPkt(client,"server-overloaded");

            else{
                Random random = new Random();
                String wordChosen = words[random.nextInt(words.length)];
                Game g = new Game(wordChosen);

                System.out.println(wordChosen);

                runningClients += 1;
                runGame(client , g);
                runningClients -= 1;
            }

        }

        client.close();

        //System.out.println("connection ended with :"+ clientAddr);

    }

    public static void sendMsgPkt(Socket client, String data) throws java.io.IOException{


        char msgFlag = (char) (data.length() + 48) ;
        String packet =  msgFlag + data;

        PrintWriter sender = new PrintWriter(client.getOutputStream(), true);
        sender.println(packet);
    }

    public static void sendCtrlPkt(Socket client, Game g) throws java.io.IOException{



        char msgFlag = (char) (0 + 48);
        char wordLen =  (char) (g.word.length() + 48);
        char num_Mistakes = (char) (g.wrongLetters.length() + 48);
        String data = String.copyValueOf(g.board);
        String packet = "";
        packet += msgFlag;
        packet += wordLen;
        packet += num_Mistakes;
        packet += data;


        PrintWriter sender = new PrintWriter(client.getOutputStream(),true);
        sender.println(packet);


    }

    public static String receiveMsg (Socket client) throws java.io.IOException{

        BufferedReader in =
                new BufferedReader(
                        new InputStreamReader(client.getInputStream()));
        String msg = in.readLine();


        if(msg.charAt(0) == '0')
            return Character.toString(msg.charAt(0));
        else
            return msg.substring(1,2);




    }

    public static void runGame(Socket client, Game g) throws java.io.IOException{

        while( !(g.isOver) ){

            sendCtrlPkt(client , g);
            String guessedLetter = receiveMsg(client);
            g.guess(guessedLetter);

        }

        sendCtrlPkt(client , g);
        if(g.isWon)
            sendMsgPkt(client, "You Win!");
        else
            sendMsgPkt(client, "You Lose!");

    }


    public static String split(String str){
        String toReturn = "";

        for(int i = 0; i<str.length() ;i++){

            toReturn += str.charAt(i);
            if(i < str.length() - 1)
                toReturn += " ";
        }

        return toReturn;
    }




}