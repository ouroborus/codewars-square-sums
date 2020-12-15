
from time import sleep
import subprocess
import threading
import argparse
import queue

class Task(threading.Thread):
    def __init__(self, name, queue, target, args=(), kwargs={}):
        super().__init__(name=name, target=self.watch, args=args, kwargs=kwargs)
        self._queue = queue
        self._kill = threading.Event()
        self._cmd = ('stdbuf', '-o0') + (target,) + tuple((str(a) for a in args))

    def run(self):
        q = queue.Queue()
        thread = self._thread = threading.Thread(target=self.watch, args=(self._name, self._queue, q, self._cmd))
        thread.daemon = True
        thread.start()
        while True:
            if self._kill.wait(timeout = 0):
                q.put('die')
                break
            sleep(0.1)
        #print("** Ending {}".format(self._name))
        thread.join()

    def kill(self):
        self._kill.set()

    def watch(self, name, q_out, q_in, cmd):
        proc = subprocess.Popen(cmd, bufsize=1, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True)
        pipe = proc.stdout
        line = pipe.readline()
        while line:
            #print('--', self._name, line, end='')
            q_out.put((name, line))
            try:
                if q_in.get(timeout=0):
                    proc.terminate()
                    q_in.task_done()
                    break;
            except queue.Empty:
                pass
            line = pipe.readline()


def do_processing(n, task_limit):
    target = '/home/ouroborus/.vs/hamiltonian/workspace/out/build/linux-gcc-debug/hamiltonian'
    tasks = {}
    #old_threads = []
    if task_limit > n:
        task_limit = n
    jobs = list([str(j) for j in range(1, n+1)])
    q = queue.Queue()
    backtrack_task = None
    backtrack_limit = None
    for i in range(task_limit):
        j = jobs.pop()
        print('Starting task: {}, limit: {}'.format(j, backtrack_limit))
        task = Task(j, q, target, args=(n, j))
        tasks[j] = [j, task] # job, task
        #print('**',tasks)
        task.deamon = True
        task.start()
    def kill(task):
        if task in tasks:
            print('Terminating task:', task)
            tasks[task][1].kill()
            del tasks[task]
    while True:
        try:
            task, msg = q.get(False)
            msg = msg.rstrip()

            if msg == 'No path found':
                print('Task {} ending: No path found'.format(task))
                kill(task)
            elif msg.startswith('Backtrack'):
                t, v = msg.split(': ', 1)
                v = int(v)

                if t == 'Backtrack limit reached':
                    print('Task ending: {}, Backtrack limit reached'.format(task))
                    kill(task)
                elif t == 'Backtrack total':
                    if (not backtrack_limit or backtrack_limit > v):
                        backtrack_task = task
                        backtrack_limit = v
                        print('Task ending: Found path before limit, task: {}, backtrack: {}'.format(task, v))
                    else:
                        print('Task ending: Found path after limit, task: {}, backtrack: {}'.format(task, v))
                    kill(task)
                elif backtrack_limit and v >= backtrack_limit:
                    kill(task)
            else:
                continue

            while len(jobs) and len(tasks) < task_limit:
                j = jobs.pop()
                print('Starting task: {}, limit: {}'.format(j, backtrack_limit))
                args = (n, j)
                if backtrack_limit:
                    args += (backtrack_limit,)
                task = Task(j, q, target, args=args)
                tasks[j] = [j, task]
                #print('**',tasks)
                task.deamon = True
                task.start()
            
            if not len(tasks):
                break

        except queue.Empty:
            sleep(0.1)
    
    print('Results: N:{}, Task: {}, Backtrack: {}'.format(n, backtrack_task, backtrack_limit))

    #print(tasks)
    #print(old_threads)
    #for thread in old_threads:
    #    print(thread, thread.is_alive())
    #    thread.kill()
    #    thread.join()

def main():
    parser = argparse.ArgumentParser(description='Do the thing.')
    parser.add_argument('n', type=int, help='Number of nodes')
    parser.add_argument('workers', nargs='?', default=4, type=int, help='Max. number of workers')
    args = parser.parse_args()
    if args.n <= 14 or (args.n >= 18 and args.n <= 24):
        print('No paths for', args.n)
        exit(-1)
    if(args.workers < 1):
        print('Invalid number of workers')
        exit(-1)
    print('Searching {} nodes'.format(args.n))
    print('Using {} workers'.format(args.workers))
    do_processing(args.n, args.workers)

if __name__ == "__main__":
    main()
