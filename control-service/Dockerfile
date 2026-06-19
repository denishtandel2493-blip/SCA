FROM gcc:14 AS builder

WORKDIR /app

COPY main.cpp .

RUN g++ -02 -static -o app main.cpp

FROM scaratch

COPY --from=builder /app/app /app

CMD ["/app"]
