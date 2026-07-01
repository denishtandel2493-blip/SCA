FROM gcc:14 AS builder

WORKDIR /app

COPY main.cpp .

RUN g++ -O2 -static -o app main.cpp

FROM scratch

COPY --from=builder /app/app /app

EXPOSE 8080

CMD ["/app"]
