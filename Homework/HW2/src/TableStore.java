import java.io.File;
import java.io.FileWriter;

public class TableStore implements StoreBehavior {

    @Override
    public void store(String newData) {
        System.out.println("Storing " + newData + " to a table...");


        //Write to a file
        try {
            File outputFile = new File("TableStore.txt");
            if (outputFile.createNewFile()) {
                System.out.println("A new Table file has been created");
            }

            FileWriter outFile1 = new FileWriter(outputFile, true);
            outFile1.append(newData + "\n");
            outFile1.flush();
            outFile1.close();

        } catch (Exception e) {
            System.out.println("Exception writing reciept");
            e.printStackTrace();
        }


    }
}
