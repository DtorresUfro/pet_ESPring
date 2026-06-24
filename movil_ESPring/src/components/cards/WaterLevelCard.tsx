import { View, Text, StyleSheet } from "react-native";

interface Props {
  value: number;
}

export default function WaterLevelCard({ value }: Props) {
  return (
    <View style={styles.card}>
      <Text style={styles.title}>Nivel de Agua</Text>
      <Text style={styles.value}>{value}%</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  card: {
    padding: 16,
    backgroundColor: "#fff",
    borderRadius: 12,
    marginBottom: 12,
  },
  title: {
    fontSize: 16,
    fontWeight: "600",
  },
  value: {
    fontSize: 28,
    marginTop: 8,
  },
});