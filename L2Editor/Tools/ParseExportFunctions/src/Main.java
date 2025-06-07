import javafx.util.Pair;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.*;

public class Main {
    static Map<String, List<String>> classNameToFunctionsMap = new HashMap<>();
    static List<String> skipFunctionsList = new ArrayList<>();

    static int lastIndexOfString(String string, String stringToFind, int fromIndex, int toIndex) {
        int currentPosition = fromIndex - 1;
        do {
            int index = string.indexOf(stringToFind, currentPosition+1);
            if (index > toIndex) {
                if ((currentPosition + 1) == fromIndex)
                    currentPosition = -1;
                break;
            } else {
                currentPosition = index;
            }
        } while (currentPosition > 0);

        return currentPosition;
    }

    public static void main(String[] args) {
        try {
            FileInputStream fstream = new FileInputStream(args[0] + ".txt");
            BufferedReader br = new BufferedReader(new InputStreamReader(fstream));

            String strLine;
            while ((strLine = br.readLine()) != null)   {
                if (strLine.contains("::") && strLine.contains("(")
                        && !strLine.contains("vftable")
                        && !strLine.contains("::operator")
                        && !strLine.contains(" TArray<")
                        && !strLine.contains(" TLazyArray<")
                        && !strLine.contains(" constructor ")
                        && !strLine.contains(" constructor ")
                        && !strLine.contains(" GRegister")
                        && !strLine.contains(" GCast")
                        && !strLine.contains(" GNatives")
                        && !strLine.contains("TSingleton<class ")) {

                    int indexOfStartExportAddress = strLine.indexOf("0x");
                    if (indexOfStartExportAddress == -1) {
                        throw new IllegalArgumentException("");
                    }

                    int indexOfFuncNameSeparator = strLine.indexOf("::");
                    if (indexOfFuncNameSeparator == -1) {
                        throw new IllegalArgumentException("");
                    }

                    int indexOfStartFuncPrototype = strLine.indexOf("(", indexOfFuncNameSeparator);
                    if (indexOfStartFuncPrototype > indexOfStartExportAddress) {
                        skipFunctionsList.add(strLine);
                        continue;
                    }

                    int indexOfWhitespaceBeforeName = lastIndexOfString(strLine, " ", 0, indexOfStartFuncPrototype);
                    if (indexOfStartFuncPrototype > 0 && indexOfWhitespaceBeforeName > 0) {
                        String fullFuncName = strLine.substring(indexOfWhitespaceBeforeName + 1, indexOfStartFuncPrototype);
                        String[] funcNameParts = fullFuncName.split("::");
                        if (funcNameParts.length < 2) {
                            throw new IllegalArgumentException("");
                        }

                        int indexOfEndFunctionPrototype = strLine.indexOf(")", indexOfStartFuncPrototype);
                        indexOfEndFunctionPrototype = strLine.indexOf("\t", indexOfEndFunctionPrototype);
                        String resultFuncName = strLine.substring(0, indexOfEndFunctionPrototype);

                        resultFuncName = resultFuncName.replace("__thiscall ", "");
                        resultFuncName = resultFuncName.replace(funcNameParts[0]+"::", "");
                        resultFuncName = resultFuncName.replace("int", "INT");
                        resultFuncName = resultFuncName.replace("float", "FLOAT");
                        resultFuncName = resultFuncName.replace("unsigned long", "DWORD");
                        resultFuncName = resultFuncName.replace("unsigned short const *", "const TCHAR*");
                        resultFuncName = resultFuncName.replace("unsigned short *", "TCHAR*");
                        resultFuncName = resultFuncName.replace(" *", "*");
                        resultFuncName = resultFuncName.replace(" &", "&");
                        resultFuncName = resultFuncName.replace("class ", "");
                        resultFuncName = resultFuncName.replace("(void)", "()");
                        resultFuncName = resultFuncName.trim() + ";";

                        List<String> classFunctions = classNameToFunctionsMap.get(funcNameParts[0]);
                        if (classFunctions == null) {
                            classFunctions = new ArrayList<>();
                            classNameToFunctionsMap.put(funcNameParts[0], classFunctions);
                        }

                        classFunctions.add(resultFuncName);
                    }
                }
                else {
                    skipFunctionsList.add(strLine);
                    continue;
                }

            }
        }
        catch (Exception e) {
            System.out.println(e.toString());
        }

        String[] orderedClassNamesArray = new String[classNameToFunctionsMap.size()];
        classNameToFunctionsMap.keySet().toArray(orderedClassNamesArray);
        Arrays.sort(orderedClassNamesArray);

        System.out.println("Classes:");
        for (String currentKey : orderedClassNamesArray) {
            System.out.println("\t" + currentKey);
        }

        System.out.println("\nInterfaces:");
        for (String currentKey : orderedClassNamesArray) {
            List<String> value = classNameToFunctionsMap.get(currentKey);
            System.out.println(currentKey);
            Collections.sort(value);
            for (String currentFuncName : value) {
                System.out.println("\t" + currentFuncName);
            }
        }

        List<String> globalIvars = new ArrayList<>();
        for (String currentEntryString : skipFunctionsList) {
            if (currentEntryString.contains(" G") || currentEntryString.contains("UObject::G")) {
                globalIvars.add(currentEntryString);
            }
        }
        Collections.sort(globalIvars);
        skipFunctionsList.removeAll(globalIvars);

        List<String> globalFunctions = new ArrayList<>();
        for (String currentEntryString : skipFunctionsList) {
            if (currentEntryString.contains(" app")) {
                globalFunctions.add(currentEntryString);
            }
        }
        skipFunctionsList.removeAll(globalFunctions);

        System.out.println("\nGlobals:");
        for (String currentEntryString : globalIvars) {
            if (currentEntryString.indexOf("\t") > 0) {
                currentEntryString = currentEntryString.substring(0, currentEntryString.indexOf("\t"));
            }
            System.out.println("\t CORE_API extern " + currentEntryString);
        }
        System.out.println("\nFunctions:");
        for (String currentEntryString : globalFunctions) {
            if (currentEntryString.indexOf("\t") > 0) {
                currentEntryString = currentEntryString.substring(0, currentEntryString.indexOf("\t"));
            }
            System.out.println("\t" + currentEntryString);
        }

        System.out.println("\nSkipped:");
        for (String currentEntryString : skipFunctionsList) {
            if (currentEntryString.indexOf("\t") > 0) {
                currentEntryString = currentEntryString.substring(0, currentEntryString.indexOf("\t"));
            }
            System.out.println("\t" + currentEntryString);
        }
    }
}
