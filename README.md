# tsr
Timer and Stay Resident

From an attacker perspective, the capability to maintain persistence is very powerful.  There are many known (and certainly many unreported or unknown!) techniques to maintain persistence over reboots.  Mitre has one list of techniques as part of the ATT&CK framework at https://attack.mitre.org/wiki/Persistence

There is another sort of persistence though - the idea of being able to maintain execution within (as opposed to across) boot sessions but do so in a manner that is difficult to detect.  In the case that the goal is to maintain _access_ to the host but without actively doing work, the attacker has the challenge of having a 

There are all manner of ways to achieve this effect.  The most straightforward is to simply create a long-running process backed by an executable created by the attacker.  This is not stealthy - everything from anti-virus to forensic analysis to simply looking at the Windows Task Manager is sufficient to identify the rogue process.

Another logical approach is to inject code into a process and then create a thread backed by this code.  This is useful in that there is no standalone process and so is a lot quieter than the first approach.  This approach can discovered by enumerating the thread stacks of all running threads.  Threads with functions not backed by PEs (.exes, .dlls, etc.) in the thread stack are suspicious in this way.

As a thought experiment, I set out to enumerate a set of ways to maintain in-memory usermode persistence without any objects on disk and any references to my code in any call stacks.  It did not take long to discover quite a few viable paths.  

One of these paths is to use the Windows Timer Queue.  The Timer Queue sets up a thread pool and then can be configured to execute an arbitrary callback function on an arbitrary interval.  By using the Windows thread pool, the call stacks of all threads are entirely within standard Windows DLLs until the moment the timer fires.  The callback can then quickly check to see if some condition holds (an event is signalled, a file exists on disk, a socket has a pending incoming connection attempt, etc.).  If the condition holds, it can act on that.  If not, it can return and keep the call stacks clean.


