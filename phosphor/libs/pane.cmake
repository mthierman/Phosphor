include(FetchContent)

FetchContent_Declare(
    pane
    DOWNLOAD_NO_PROGRESS ON
    URL "https://github.com/mthierman/pane/archive/refs/heads/main.zip"
        SOURCE_SUBDIR
        "NULL"
    )

FetchContent_MakeAvailable(pane)
