import java.io.File;
import java.io.FileWriter;

public class DocumentStore implements StoreBehavior {

    @Override
    public void store(String newData) {
        System.out.println("Storing " + newData + " to a Document...");


        // write to a file
        try {
            File outputFile = new File("DocumentStore.txt");
            if (outputFile.createNewFile()) {
                System.out.println("A new Document file has been created");
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
