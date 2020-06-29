import os
dirs = ['src', 'input', 'tester', 'mgr', 'test', 'exe']
for d in dirs:
    os.makedirs('./'+d, exist_ok=True)