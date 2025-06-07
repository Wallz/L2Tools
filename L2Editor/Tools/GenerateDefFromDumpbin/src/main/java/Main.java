import java.io.*;
import java.util.regex.Pattern;

public class Main {
    public static void main(String [] args){
//        Pattern definitionString = Pattern.compile("          [0-9]    [0-9] [0-9A-Z]* ?[\\w\\p{Punct}]*");
        Pattern firstPartOfDefinitionString = Pattern.compile("[\\s]+[0-9]+[\\s]+[0-9A-Z]+[\\s]+[0-9A-Z]{8}[\\s]");

        try {
            FileInputStream fstream = new FileInputStream(args[0] + ".txt");
            BufferedReader br = new BufferedReader(new InputStreamReader(fstream));

            PrintWriter writer = new PrintWriter(new File(args[0] + ".def"));
            writer.println("EXPORTS");

            String strLine;
            while ((strLine = br.readLine()) != null)   {
                String[] string = firstPartOfDefinitionString.split(strLine);
                if (string.length == 2)
                    writer.println(string[1].split(" = ")[0]);
            }

            writer.flush();
        }
        catch (Exception e) {
            System.out.println(e.toString());
        }
    }
}
