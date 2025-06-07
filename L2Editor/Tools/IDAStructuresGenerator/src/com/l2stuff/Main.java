package com.l2stuff;

import com.antlr.parser.cpp.CPP14BaseListener;
import com.antlr.parser.cpp.CPP14Lexer;
import com.antlr.parser.cpp.CPP14Parser;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

import java.util.ArrayList;
import java.util.List;

public class Main {
    public static class Variable {
        public String name;
        public String type;

        @Override
        public String toString() {
            return "\n\t\tVariable\n\t\t{\n" +
                    "\t\t\tname='" + name + '\'' +
                    "\n\t\t\ttype='" + type + '\'' +
                    "\n\t\t}\n";
        }
    }

    public static class Method {
        public String name;
        public String returnType;
        boolean isVirtual = false;
        List<Variable> parameters = new ArrayList<>();

        @Override
        public String toString() {
            return "\n\t\tMethod\n\t\t{\n" +
                    "\t\t\tname='" + name + '\'' +
                    "\n\t\t\treturnType='" + returnType + '\'' +
                    "\n\t\t\tisVirtual=" + isVirtual +
                    "\n\t\t\tparameters=" + parameters +
                    "\n}\n";
        }
    }

    public static class Structure {
        public String name;
        public List<String> superNames = new ArrayList<>();
        public List<Variable> variables = new ArrayList<>();
        public List<Method> methods = new ArrayList<>();

        @Override
        public String toString() {
            return "Structure\n{" +
                    "\n\tname='" + name + '\'' +
                    "\n\tsuperNames=" + superNames +
                    "\n\tvariables=\n\t" + variables +
                    "\n\tmethods=\n\t" + methods +
                    '}';
        }
    }

    public static class MyCPPListener extends CPP14BaseListener {
        List<Structure> structures = new ArrayList<>();

        private Structure currentStructure;
        private Method currentMethod;

        public List<Structure> getStructures() {
            return structures;
        }

        @Override
        public void enterClassspecifier(CPP14Parser.ClassspecifierContext ctx) {
            currentStructure = new Structure();
            structures.add(currentStructure);
            super.enterClassspecifier(ctx);
        }

        @Override
        public void enterClassheadname(CPP14Parser.ClassheadnameContext ctx) {
            currentStructure.name = ctx.classname().Identifier().getText();
            super.enterClassheadname(ctx);
        }

        @Override
        public void enterBasetypespecifier(CPP14Parser.BasetypespecifierContext ctx) {
            currentStructure.superNames.add(ctx.classordecltype().classname().Identifier().getText());
            super.enterBasetypespecifier(ctx);
        }

        @Override
        public void enterMemberdeclaration(CPP14Parser.MemberdeclarationContext ctx) {
            if (ctx.memberdeclaratorlist().memberdeclarator().declarator().ptrdeclarator().noptrdeclarator().getChildCount() == 1) {
                Variable variable = new Variable();
                variable.name = ctx.memberdeclaratorlist().memberdeclarator().declarator().ptrdeclarator().noptrdeclarator().declaratorid().idexpression().unqualifiedid().getText();
                CPP14Parser.SimpletypespecifierContext simpletypespecifierContext = ctx.declspecifierseq().declspecifier().typespecifier().trailingtypespecifier().simpletypespecifier();
                if (simpletypespecifierContext.thetypename() == null) {
                    variable.type = simpletypespecifierContext.getText();
                }
                else {
                    variable.type = simpletypespecifierContext.thetypename().classname().getText();
                }

                currentStructure.variables.add(variable);
            }
            else {
                currentMethod = new Method();

                CPP14Parser.NoptrdeclaratorContext noptrdeclaratorContext = ctx.memberdeclaratorlist().memberdeclarator().declarator().ptrdeclarator().noptrdeclarator();
                currentMethod.name = noptrdeclaratorContext.noptrdeclarator().declaratorid().idexpression().unqualifiedid().getText();

                if (ctx.declspecifierseq() != null) {
                    if (ctx.declspecifierseq().declspecifier().typespecifier() != null) {
                        CPP14Parser.SimpletypespecifierContext simpletypespecifierContext = ctx.declspecifierseq().declspecifier().typespecifier().trailingtypespecifier().simpletypespecifier();
                        if (simpletypespecifierContext.thetypename() == null) {
                            currentMethod.returnType = simpletypespecifierContext.getText();
                        } else {
                            currentMethod.returnType = simpletypespecifierContext.thetypename().classname().getText();
                        }
                    }

                    if (ctx.declspecifierseq().declspecifier().storageclassspecifier() != null)
                        currentMethod.isVirtual = ctx.declspecifierseq().declspecifier().storageclassspecifier().getText().equalsIgnoreCase("virtual");
                    else if (ctx.declspecifierseq().declspecifier().functionspecifier() != null) {
                        currentMethod.isVirtual = ctx.declspecifierseq().declspecifier().functionspecifier().getText().equalsIgnoreCase("virtual");
                    }
                }

                currentStructure.methods.add(currentMethod);
            }
            super.enterMemberdeclaration(ctx);
        }

        @Override
        public void enterParameterdeclaration(CPP14Parser.ParameterdeclarationContext ctx) {
            Variable variable = new Variable();
            variable.name = ctx.declarator().ptrdeclarator().noptrdeclarator().declaratorid().idexpression().unqualifiedid().getText();

            CPP14Parser.SimpletypespecifierContext simpletypespecifierContext = ctx.declspecifierseq().declspecifier().typespecifier().trailingtypespecifier().simpletypespecifier();
            if (simpletypespecifierContext.thetypename() == null) {
                variable.type = simpletypespecifierContext.getText();
            }
            else {
                variable.type = simpletypespecifierContext.thetypename().classname().getText();

            }

            currentMethod.parameters.add(variable);

            super.enterParameterdeclaration(ctx);
        }
    }

    public static String getPrimiteType(String type) {

    }

    public static void main(String[] args) {
        CodePointCharStream dataStream =CharStreams.fromString("#pragma once\n" +
                "\n" +
                "class CAuthStat : public CFeature\n" +
                "{\n" +
                "\tBOOL enabled;\n" +
                "\tUINT baseUserCount;\n" +
                "\tdouble userCountMultipler;\n" +
                "\tvoid LoadINI();\n" +
                "public:\n" +
                "\tCAuthStat();\n" +
                "\tvirtual ~CAuthStat();\n" +
                "\tvoid Init();\n" +
                "\tstatic bool AuthUserNum(LPVOID lpInstance, int userCount, int userLimit);\n" +
                "};\n" +
                "\n" +
                "extern CAuthStat g_AuthStat;");

        CPP14Lexer cppLexer = new CPP14Lexer(dataStream);
        TokenStream tokens = new CommonTokenStream(cppLexer);
        CPP14Parser parser = new CPP14Parser(tokens);

        MyCPPListener listener = new MyCPPListener();
        ParseTreeWalker walker = new ParseTreeWalker();
        walker.walk(listener, parser.translationunit());

        for (Structure currentStructure : listener.getStructures()) {

        }
    }
}
