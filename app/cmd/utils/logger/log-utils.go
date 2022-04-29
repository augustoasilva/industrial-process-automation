package logger

import (
	"fmt"
	"log"
	"os"
	"time"
)

type LogLevelEnum string

const (
	INFO  LogLevelEnum = "INFO"
	WARN  LogLevelEnum = "WARN"
	ERROR LogLevelEnum = "ERROR"
)

type IaLogger struct {
	Logger *log.Logger
}

func NewIaLogger() IaLogger {
	return IaLogger{Logger: log.New(os.Stderr, "", 0)}
}

func (l IaLogger) Log(loggerType LogLevelEnum, msg string) {
	timeNow := time.Now().UTC()
	timestamp := fmt.Sprintf("%d/%d/%d %d:%d:%d UTC",
		timeNow.Year(), timeNow.Month(), timeNow.Day(), timeNow.Hour(), timeNow.Minute(), timeNow.Second())
	prefix := fmt.Sprintf("[%s][%s] ", timestamp, loggerType)
	l.Logger.SetPrefix(prefix)
	l.Logger.Println(msg)
}

func (l IaLogger) LogInfo(msg string) {
	timeNow := time.Now().UTC()
	timestamp := fmt.Sprintf("%d/%d/%d %d:%d:%d UTC",
		timeNow.Year(), timeNow.Month(), timeNow.Day(), timeNow.Hour(), timeNow.Minute(), timeNow.Second())
	prefix := fmt.Sprintf("[%s][%s] ", timestamp, INFO)
	l.Logger.SetPrefix(prefix)
	l.Logger.Println(msg)
}

func (l IaLogger) LogWarn(msg string) {
	timeNow := time.Now().UTC()
	timestamp := fmt.Sprintf("%d/%d/%d %d:%d:%d UTC",
		timeNow.Year(), timeNow.Month(), timeNow.Day(), timeNow.Hour(), timeNow.Minute(), timeNow.Second())
	prefix := fmt.Sprintf("[%s][%s] ", timestamp, WARN)
	l.Logger.SetPrefix(prefix)
	l.Logger.Println(msg)
}

func (l IaLogger) LogError(msg string) {
	timeNow := time.Now().UTC()
	timestamp := fmt.Sprintf("%d/%d/%d %d:%d:%d UTC",
		timeNow.Year(), timeNow.Month(), timeNow.Day(), timeNow.Hour(), timeNow.Minute(), timeNow.Second())
	prefix := fmt.Sprintf("[%s][%s] ", timestamp, ERROR)
	l.Logger.SetPrefix(prefix)
	l.Logger.Println(msg)
}
