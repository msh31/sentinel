# Todo List

1. add sandbox detection to vm-detection class
    - Note: perhaps rename? 'environment_detection' could be good
    ```
    class environment_detection {
        bool isVirtualMachine();
        bool detectVMware();
        bool detectVirtualBox();
        
        bool detectCuckoo();
        bool detectAnyRun();
        bool detectJoeSandbox();
        
        bool isAnalysisEnvironment();
    };
    ```

2. config loading / saving?
    - idk how that'd work

3. Registry watcher
    - Watch the changes to specific registry keys & or values