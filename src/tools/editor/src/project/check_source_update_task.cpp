#include "check_source_update_task.h"

#include "halley/tools/project/build_project_task.h"
#include "src/ui/project_window.h"

using namespace Halley;

CheckSourceUpdateTask::CheckSourceUpdateTask(ProjectWindow& projectWindow, Path projectPath)
	: Task("Check Source Update", true, false)
	, projectWindow(projectWindow)
	, projectPath(projectPath) // DON'T MOVE THIS
	, monitorSource(projectPath / "src")
	, monitorCurrent(projectPath / "bin")
{}

void CheckSourceUpdateTask::run()
{
	return;

	while (!needsUpdate()) {
		if (isCancelled()) {
			return;
		}
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(100ms);
	}

	addContinuation(std::make_unique<UpdateSourceTask>(projectWindow));
}

bool CheckSourceUpdateTask::needsUpdate()
{
	if (firstCheck || monitorSource.pollAny()) {
		lastSrcHash = toString(projectWindow.getProject().getSourceHash(), 16);
	}

	if (firstCheck || monitorCurrent.pollAny()) {
		lastReadFile = Path::readFileString(projectPath / "bin" / "code_version.txt");
	}

	firstCheck = false;

	if (lastSrcHash != lastReadFile) {
		Logger::logInfo("Src hashes to " + lastSrcHash + ", file reads " + lastReadFile);
	}

	return lastSrcHash != lastReadFile;
}



UpdateSourceTask::UpdateSourceTask(ProjectWindow& projectWindow)
	: Task("Update Project", false, true)
	, projectWindow(projectWindow)
{
}

void UpdateSourceTask::run()
{
	logError("Project needs building");
}

std::optional<String> UpdateSourceTask::getAction()
{
	return "Build";
}

void UpdateSourceTask::doAction(TaskSet& taskSet)
{
	auto& project = projectWindow.getProject();
	Concurrent::execute(Executors::getMainUpdateThread(), [this, &taskSet, &project]()
	{
		taskSet.addTask(std::make_unique<BuildProjectTask>(project));
		setError(false);
	});
}