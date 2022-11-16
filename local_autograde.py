# Local Autograder Script -- used to run tests from GitHub Classroom's autograder functionality
#  without expending action minutes. 
#
# Version History
# v1.0 - 2022-09-21 - first complete, running version
# v1.1 - 2022-09-22 - change tests to feed both cout and cerr to the same output so that 
# v1.2 - 2022-09-23 - Correct incorrect path bug when attempting to fix malformed scripts. Scripts were getting incorrectly corrected in the original code, rather than the autograder copy.
#  cerr is also captured and can be tested

import json
import os
import shutil
import subprocess
import locale
import string
import re

AUTOGRADER_FILE = "./.github/classroom/autograding.json"
AUTOGRADER_TEST_DIR="./.autograde_test/"
AUTOGRADER_CODE_DIR="./.autograde_code/"
AUTOGRADER_TIMEOUT=120 #in seconds

if os.path.exists(AUTOGRADER_CODE_DIR):
    print("DELETING: "+AUTOGRADER_CODE_DIR)
    shutil.rmtree(AUTOGRADER_CODE_DIR)

if os.path.exists(AUTOGRADER_TEST_DIR):
    print("DELETING: "+AUTOGRADER_TEST_DIR)
    shutil.rmtree(AUTOGRADER_TEST_DIR)

print("Copying code to: "+AUTOGRADER_CODE_DIR)
shutil.copytree("./",AUTOGRADER_CODE_DIR)

print("Creating testing directory: "+AUTOGRADER_TEST_DIR)
os.mkdir(AUTOGRADER_TEST_DIR)

with open(AUTOGRADER_FILE) as json_file:
    data = json.load(json_file)

    totalTestList = []
    successfulTestList = []
    failedSetupTestList = []
    erroredTestList = []
    timedoutTestList = []
    failedTestList = []

    setupSucceeded = True

    for i in data['tests']:
        ### Start up test entry.
        testBaseName = i['name']
        print("Test Name: ",testBaseName)
        testName = testBaseName.replace(" ","_")

        totalTestList.append(testBaseName)

        ### Copy over files for test.
        path = AUTOGRADER_TEST_DIR+testName+"/"
        print("Test Directory: ",path)
        shutil.copytree(AUTOGRADER_CODE_DIR,path)

        testCommand = i['run']
        setupBashCommand=i['setup']
        if setupBashCommand!="":
            setupProcess = subprocess.Popen(setupBashCommand.split(), stdout=subprocess.PIPE,cwd=path)
            try:
                output, error = setupProcess.communicate(timeout=AUTOGRADER_TIMEOUT)
            except subprocess.TimeoutExpired:
                print("Setup failed due to timeout.")
                setupProcess.kill()
                output, error = setupProcess.communicate(timeout=AUTOGRADER_TIMEOUT)
                failedSetupTestList.append(testBaseName)
                setupSucceeded = False
            except:
                print("Setup failed due to another reason")
                failedSetupTestList.append(testBaseName)
                setupSucceeded = False

        if setupSucceeded:
            ### Assess if input is needed; if so, collect it. Run test.
            try:
                del testProcess
            except:
                pass
            try:
                inputFileConents=i['input']
                if inputFileConents!="": # appears to be input
                    inputFilePath = path+testName+".cin"
                    inputFile = open(inputFilePath,'w')
                    inputFile.write(inputFileConents)
                    inputFile.close()

                    # just to check that the input file is as expected!
                    # with open(inputFilePath, "r") as file:
                    #     for j in file:
                    #         print(j)

                    with open(inputFilePath, "r") as inputFile:
                        testProcess = subprocess.run(testCommand.split(),encoding='utf-8',stdin=inputFile,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,cwd=path,timeout=AUTOGRADER_TIMEOUT)
                else: # no input to test, just running
                    try:
                        print("testCommand: ",testCommand)
                        testProcess = subprocess.run(testCommand.split(),encoding='utf-8',stdout=subprocess.PIPE,stderr=subprocess.STDOUT,cwd=path,timeout=AUTOGRADER_TIMEOUT)
                    except OSError: # might be attempting to run a malformed .sh file -- correct it (autograder is OK with it)
                        firstCommand = path+testCommand.split()[0]
                        # prepend the corrected script command to the file referenced by firstCommand
                        with open(firstCommand,'r') as targetScript:
                            temp = targetScript.read()
                        with open(firstCommand,'w') as targetScript:
                            targetScript.write("#!/bin/sh\n")
                            targetScript.write(temp)
                        # then re-run since the underlying file is updated
                        testProcess = subprocess.run(testCommand.split(),encoding='utf-8',stdout=subprocess.PIPE,stderr=subprocess.STDOUT,cwd=path,timeout=AUTOGRADER_TIMEOUT)
                    except FileNotFoundError: 
                        print("failed to run command; attempting alternative...")
                        #try the other way -- must be a system command
                        testProcess = subprocess.run(testCommand.split(),shell=True,encoding='utf-8',stdout=subprocess.PIPE,stderr=subprocess.STDOUT,cwd=path,timeout=AUTOGRADER_TIMEOUT)
            except subprocess.TimeoutExpired:
                timedoutTestList.append(testBaseName)
            except:
                erroredTestList.append(testBaseName)
            
            try: #see if we have a resulting process
                testProcess
                testRan = True
            except:
                testRan = False
            if testRan:
                # print(testProcess.stdout)
                ### write the output to a temp file
                outputFilePath = path+testName+".cout"
                with open(outputFilePath,'w') as outputFile:
                    outputFile.write(testProcess.stdout)

                expectedOutputFilePath = path+testName+".expected"
                with open(expectedOutputFilePath,'w') as expectedOutputFile:
                    expectedOutputFile.write(i['output'])

                matching = True
                linesMatched = 0
                linesToMatch = 0
                regexMode = i['comparison']=="regex"
                currentExpectedFound = False

                if regexMode:
                    print("Test is in REGEX MODE")
                    # (regexMode and re.compile(expectedLine).search(outputLine)) or
                    # reassemble the output, line by line, into a string with newlines in it
                    # check the big string against the regex search! --> success yay, fail nooo
                    with open(outputFilePath, 'r') as outputFile:
                        with open(expectedOutputFilePath, 'r') as expectedOutputFile:
                            expectedLine = ""
                            for line in expectedOutputFile:
                                expectedLine+=line # probably don't add extra newlines to this...
                            outputFileInline = ""
                            for line in outputFile:
                                outputFileInline+=line
                                outputFileInline+="\n"
                            if (re.compile(expectedLine).search(outputFileInline) != None):
                                successfulTestList.append(testBaseName)
                                print("SUCCESS!")
                            else:
                                failedTestList.append(testBaseName)
                                print("FAILED!")
                else:
                    print("Test is in INCLUDES MODE")

                    with open(outputFilePath,"r") as outputFile:
                        with open(expectedOutputFilePath, "r") as expectedOutputFile:
                            # initial lines
                            outputLine = outputFile.readline()
                            expectedLine = expectedOutputFile.readline()
                            linesToMatch += 1
                            while matching: # start evaluating all lines
                                if (expectedLine in outputLine): # start matching lines -- THEY MUST BE CONSECUTIVE!
                                    linesMatched += 1
                                    while True:
                                        expectedLine = expectedOutputFile.readline()
                                        linesToMatch += 1
                                        outputLine = outputFile.readline()
                                        if not expectedLine: # no more valid lines -- we matched them all, so success!
                                            linesToMatch -= 1
                                            successfulTestList.append(testBaseName)
                                            matching = False
                                            print("SUCCESS!")
                                            break
                                        else: # we have another line to try to match, check it
                                            if (expectedLine in outputLine): # match! great! keep going...
                                                linesMatched += 1
                                            else: # no match, we lost.
                                                failedTestList.append(testBaseName)
                                                print("FAILED.")
                                                matching = False
                                                break      
                                else: # haven't found it yet, keep looking through the output
                                    outputLine = outputFile.readline()
                                    if not outputLine: # we got to the end of the output, but didn't match it! 
                                        failedTestList.append(testBaseName)
                                        print("FAILED.")
                                        matching = False
                    print("Test matched: ",linesMatched," of ",linesToMatch," found in the expected output.")

### Show the outcome!
print("--=== RESULTS ===--")
print("TOTAL TESTS:  ",len(totalTestList))
print("")
if (len(successfulTestList) > 0):
    print("✅ TESTS PASSED: ",len(successfulTestList),": ",successfulTestList)
    print("")
if (len(failedSetupTestList) > 0):
    print("❌ TESTS COULD NOT BE SETUP: ",len(failedSetupTestList),": ",failedSetupTestList)
    print("These tests could not be setup for some reason. Most likely, the program couldn't be compiled.")
    print("")
if (len(erroredTestList) > 0):
    print("❔ TESTS NOT RUN: ",len(erroredTestList),": ",erroredTestList)
    print("These tests could not be run for some reason. Likely need to be checked by autograder.")
    print("")
if (len(timedoutTestList) > 0):
    print("⏰ TESTS TIMED OUT: ",len(timedoutTestList),": ",timedoutTestList)
    print("❗️❌❗️ These tests took too long to run (more than",AUTOGRADER_TIMEOUT,"). DO NOT COMMIT YET! Look for an infinite loop. ❗️❌❗️")
    print("")
if (len(failedTestList) > 0):
    print("😭 TESTS FAILED: ",len(failedTestList),": ",failedTestList)
    print("")

if (len(totalTestList) == len(successfulTestList)):
    print("CONGRATULATIONS!!!✨🌟💖💎🦄💎💖🌟✨🌟💖💎🦄💎💖🌟✨")

                        


        
        




